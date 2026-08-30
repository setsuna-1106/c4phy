# C++ + Python 计算物理工作流手册

> 面向场景：C++ 做面向对象的数值计算核心，Python 做可视化和数据分析
> 最后更新：2026-08-29
>
> 姊妹篇：[C + Python 计算物理工作流手册](C_Python_Workflow.md)（管道 / ctypes / HDF5
> 等以 C 为主线的方案）。本手册聚焦 **C++ 特有**的部分：类的封装、pybind11 绑定，
> 以及本仓库 C++ 项目的实际组织方式。

---

## 目录

1. [C、C++、Python 怎么分工](#1-c-c-python-怎么分工)
2. [C++ 数值核心设计模式（本仓库实践）](#2-c-数值核心设计模式本仓库实践)
3. [C++ → Python：数据文件交换](#3-c--python数据文件交换)
4. [Python 调用 C++ 的 4 种方案](#4-python-调用-c-的-4-种方案)
5. [C++ 项目结构与构建（本仓库规范）](#5-c-项目结构与构建本仓库规范)
6. [工作流决策指南](#6-工作流决策指南)

---

## 1. C、C++、Python 怎么分工

### 1.1 三者对比

| 维度 | C | C++ | Python |
|------|---|-----|--------|
| 抽象能力 | 过程式，手动组织模块 | 类 / 模板 / RAII，物理对象直接建模 | 类 + 鸭子类型，最灵活 |
| 数值性能 | 极快 | 极快（可零开销抽象） | 慢（靠 numpy 向量化补） |
| 出错成本 | 悬垂指针 / 手动 free 常见 | RAII 后内存错误大幅减少 | 运行时才报错 |
| 可视化 | Raylib（实时渲染） | SDL2 / ImPlot / ROOT（实时渲染） | matplotlib（出版级静态图） |
| 适合规模 | 几百行的单文件小程序 | 千行以上、多物理对象耦合 | 任何规模的胶水层与分析 |

### 1.2 什么时候从 C 升级到 C++？

看到以下信号之一，就值得切换：

1. **同一套物理被多个程序复用** —— C 里靠复制粘贴 `.c` 文件共享代码，
   C++ 里抽成类（如本仓库 `src/Physics/DoublePendulum.cpp`）。
2. **"参数 + 状态 + 方法" 开始绑在一起** —— C 里是散落的全局变量，
   C++ 里 `params() / state() / step()` 封在一个对象里，随便开 N 个实例。
3. **需要同时跑很多份模拟**（扫参数、系综平均）—— 类的多实例天然支持，
   C 的全局状态改起来非常痛苦。
4. **内存管理开始碍事** —— `std::vector` / RAII 替代 `malloc/free`。

**不需要升级的信号**：几百行以内、一次性验证算法的小程序 ——
本仓库 `project/c/` 下的多数项目就是这种，C 写得更快更直接。

一句话：**C 做计算，C++ 做建模，Python 做分析与出图。**

### 1.3 本仓库的落位

```
project/c/      单文件小程序：Raylib 直接渲染（双摆、理想气体、随机行走…）
project/cpp/    多文件面向对象项目：物理类 + SDL2 渲染层分离
project/python/ matplotlib 可视化：读 C / C++ 项目写出的 CSV
```

---

## 2. C++ 数值核心设计模式（本仓库实践）

以 `project/cpp/double_pendulum/` 为例，这个项目展示了几条值得复用的模式。

### 2.1 物理与渲染彻底分离

```
include/Physics/DoublePendulum.h   ← 纯数值，不 include 任何 SDL2 头
src/Physics/DoublePendulum.cpp
main.cpp                           ← 渲染层，调用物理类的接口
```

物理类只认 `DPParams / DPState / step()`，不依赖任何渲染库。好处：

- 换渲染库（SDL2 → Raylib → 终端 ASCII）不动物理代码；
- 可以在无窗口环境（服务器 / CI）里 headless 跑模拟、直接写数据文件；
- 单元测试不需要初始化图形库。

### 2.2 参数与状态分离

```cpp
struct DPParams {                  // 不随时间变的"系统设定"
    double l1, l2;                 // 摆长 [m]
    double m1, m2;                 // 摆锤质量 [kg]
    double g;                      // 重力加速度 [m/s^2]
};

struct DPState {                   // 随时间演化的"相空间点"
    double th1, th2;               // 角度 [rad]
    double w1, w2;                 // 角速度 [rad/s]
};
```

扫参数（改 `params`、保 `state`）与做系综（改 `state`、保 `params`）
是计算物理最高频的两类实验，结构上分开后两者都只剩一行构造函数。

### 2.3 状态空间上的运算符重载 → 通用 RK4

RK4 需要 `y + k*f(y)` 这种线性组合。给状态定义加法和数乘：

```cpp
inline DPState operator+(const DPState& a, const DPState& b) {
    return {a.th1 + b.th1, a.th2 + b.th2, a.w1 + b.w1, a.w2 + b.w2};
}

inline DPState operator*(double k, const DPState& a) {
    return {k * a.th1, k * a.th2, k * a.w1, k * a.w2};
}
```

之后 RK4 本体与"解的是什么方程"完全解耦：

```cpp
void DoublePendulum::step(double dt) {
    const DPState k1 = deriv(s_);
    const DPState k2 = deriv(s_ + (0.5 * dt) * k1);
    const DPState k3 = deriv(s_ + (0.5 * dt) * k2);
    const DPState k4 = deriv(s_ + dt * k3);
    s_ = s_ + (dt / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    t_ += dt;
}
```

换成三体、 Lorenz，只需换 `deriv()`，积分器一个字不改。
（状态维数高、或想统一支持任意维数时，再升级成 `std::valarray<double>`
或 Eigen 的 `VectorXd`。）

### 2.4 守恒量做数值正确性监测

```cpp
double energy_error() const;   // 相对初始能量的漂移 (E - E0) / E0
```

`energy_error()` 在运行界面实时显示。能量漂移突然增大 = 步长过大或
方程写错的早期警报 —— 比肉眼看轨迹"像不对劲"可靠得多。
任何守恒系统（能量 / 动量 / 粒子数）都值得配这样一个仪表。

### 2.5 其它细节约定

- 头文件保护统一 `#pragma once`；include 顺序：C 标准库 → 项目内模块。
- 只读访问返回 `const&`（`const DPState& state() const`），避免拷贝。
- 传参用 `const T&`，返回值用值语义 —— 小结构体让编译器优化，别过度用指针。
- 单位制写进注释（上面 SI 单位约定），混乱的单位制是数值 bug 重灾区。

---

## 3. C++ → Python：数据文件交换

与 C+Python 手册相同的思路：**计算与可视化解耦，靠文件中转**。
本仓库 C 系项目的现成例子：`project/c/perturbed_damped_pendulum` 写出
`theta-t.csv`，`project/python/perturbed_damped_pendulum/main.py` 读同一份数据出图。

### 3.1 CSV（首选，人可读，数据量 < 几十 MB）

C++ 端写：

```cpp
#include <fstream>

void dump_trajectory(const std::vector<double>& t,
                     const std::vector<double>& th,
                     const char* fname) {
    std::ofstream out(fname);
    out << "t,theta\n";                       // 表头，方便 Python 端 skiprows
    for (std::size_t i = 0; i < t.size(); ++i)
        out << t[i] << ',' << th[i] << '\n';
}
```

Python 端读（本仓库统一用 `pathlib` 相对定位，禁止绝对路径）：

```python
from pathlib import Path
import numpy as np

csv = Path(__file__).resolve().parents[2] / "cpp" / "my_project" / "theta-t.csv"
data = np.loadtxt(csv, delimiter=",", skiprows=1)
t, theta = data[:, 0], data[:, 1]
```

### 3.2 原始二进制（大数组：场数据、百万粒子轨迹）

C++ 端：

```cpp
#include <fstream>

void write_field(const char* fname, const double* u, std::size_t n) {
    std::ofstream out(fname, std::ios::binary);
    std::uint64_t nn = n;                      // 先写长度，Python 端才知道形状
    out.write(reinterpret_cast<const char*>(&nn), sizeof nn);
    out.write(reinterpret_cast<const char*>(u),
              static_cast<std::streamsize>(n * sizeof(double)));
}
```

Python 端：

```python
import numpy as np

with open("field.bin", "rb") as f:
    n = np.fromfile(f, dtype=np.uint64, count=1)[0]
    u = np.fromfile(f, dtype=np.float64)
assert u.size == n
```

注意两端都用小端 `float64`（x86 / Apple Silicon 上默认就是）。
多物理量、多时间片、带元数据时升级 HDF5（C++ 端 HighFive / h5py-cpp，
Python 端 `h5py`）。

### 3.3 约定

- 数据文件（`*.csv` / `*.bin`）不进 git（`.gitignore` 已忽略 `*.csv`）。
- 文件名用蛇形命名（`theta-t.csv` 是历史遗留，新文件请用 `theta_t.csv`）。
- 写文件的职责放在渲染层 / 主程序，**不要**放进物理类 —— 保持 2.1 的分离。

---

## 4. Python 调用 C++ 的 4 种方案

反方向集成：Python 做主程序（Jupyter 里交互调参），性能瓶颈留在 C++。

### 决策树

```
想从 Python 里用什么？
│
├─ 只是跑一个独立程序，拿结果文件
│   └─ 方案 A：subprocess + 数据文件（零绑定成本）
│
├─ 几个自由函数（能量、单步积分、残差）
│   ├─ 不想装任何东西 → 方案 B：extern "C" + ctypes
│   └─ 愿意 pip install pybind11 → 方案 C：pybind11（同样能导出函数）
│
└─ 完整的 C++ 类（构造 DoublePendulum、逐步 step、读 state）
    └─ 方案 C：pybind11（标准做法；新项目也可看 nanobind）
```

### 方案 A：subprocess + 数据文件

C++ 程序按 2.1 的分离设计成可以 headless 运行（如 `./sim --dt 0.001 --out traj.csv`），
Python 侧：

```python
import subprocess
subprocess.run(["make", "-C", "../cpp/my_project"], check=True)
subprocess.run(["../cpp/my_project/build/sim", "--dt", "0.001",
                "--out", "traj.csv"], check=True)
# 之后 loadtxt 读 traj.csv
```

优点：零绑定代码，C++ 端不需要为 Python 做任何改动。
缺点：每次调用起一个进程，不适合细粒度交互。

### 方案 B：`extern "C"` + ctypes（函数接口）

C++ 的名字改写（name mangling）会让 ctypes 找不到符号，用 `extern "C"`
关掉它：

```cpp
// bridge.cpp
#include <cmath>

extern "C" {

double sho_energy(double k, double m, double x, double v) {
    return 0.5 * m * v * v + 0.5 * k * x * x;
}

// 数组入参：传指针 + 长度，绝不抛异常跨越边界
void rk4_steps(double* th, double* w, int n,
               double dt, int steps, double g, double l) {
    for (int s = 0; s < steps; ++s) {
        /* 单摆 RK4，略 */
    }
}

}  // extern "C"
```

编译成动态库：

```bash
# macOS
c++ -O2 -shared -fPIC -o libbridge.dylib bridge.cpp
# Linux
c++ -O2 -shared -fPIC -o libbridge.so bridge.cpp
```

Python 侧（标准库，零依赖）：

```python
import ctypes
import numpy as np

lib = ctypes.CDLL("./libbridge.dylib")

lib.sho_energy.argtypes = [ctypes.c_double] * 4
lib.sho_energy.restype = ctypes.c_double

lib.rk4_steps.argtypes = [
    ctypes.POINTER(ctypes.c_double),   # th (in/out)
    ctypes.POINTER(ctypes.c_double),   # w  (in/out)
    ctypes.c_int, ctypes.c_double, ctypes.c_int,
    ctypes.c_double, ctypes.c_double,
]
lib.rk4_steps.restype = None

th = np.zeros(1000)
w = np.zeros(1000)
lib.rk4_steps(
    th.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    w.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    1000, 0.001, 10000, 9.81, 1.0,
)
```

**两条铁律**：跨边界的函数必须 `extern "C"` 且**不抛 C++ 异常**
（catch 住一切再返回错误码）；类型映射速查见 C+Python 手册 §3.3（完全通用）。

### 方案 C：pybind11（类接口，推荐）

能把本仓库的 `DoublePendulum` 类**原样**暴露给 Python，保留构造、方法、属性：

```cpp
// bindings.cpp
#include <pybind11/pybind11.h>

#include "Physics/DoublePendulum.h"

namespace py = pybind11;

PYBIND11_MODULE(pendulum, m) {
    m.doc() = "double pendulum physics core";

    py::class_<DPParams>(m, "DPParams")
        .def(py::init<>())
        .def_readwrite("l1", &DPParams::l1)
        .def_readwrite("l2", &DPParams::l2)
        .def_readwrite("m1", &DPParams::m1)
        .def_readwrite("m2", &DPParams::m2)
        .def_readwrite("g",  &DPParams::g);

    py::class_<DoublePendulum>(m, "DoublePendulum")
        .def(py::init<const DPParams&, const DPState&>())
        .def("step", &DoublePendulum::step, py::arg("dt"))
        .def("energy", &DoublePendulum::energy)
        .def("energy_error", &DoublePendulum::energy_error)
        .def("bob1", &DoublePendulum::bob1)
        .def("bob2", &DoublePendulum::bob2);
}
```

编译（macOS 关键是 `-undefined dynamic_lookup`，不链接 libpython）：

```bash
pip install pybind11
c++ -O2 -std=c++17 -shared -undefined dynamic_lookup \
    -Iproject/cpp/double_pendulum/include \
    $(python3 -m pybind11 --includes) bindings.cpp \
    -o pendulum$(python3-config --extension-suffix)
# Linux：把 -undefined dynamic_lookup 换成 -fPIC（其余相同）
```

Python 里就像本地类一样用：

```python
import numpy as np
import matplotlib.pyplot as plt
import pendulum

p = pendulum.DPParams()
p.l1, p.l2, p.m1, p.m2, p.g = 1.0, 0.8, 1.0, 1.0, 9.81
sim = pendulum.DoublePendulum(p, init_state)      # 在 Jupyter 里交互调参

t, err = [], []
for i in range(200000):
    sim.step(1e-3)
    t.append(i * 1e-3)
    err.append(sim.energy_error())

plt.semilogy(t, np.abs(err)); plt.xlabel("t [s]"); plt.ylabel("|ΔE/E₀|")
```

`pybind11/stl.h` 头还能自动转换 `std::vector<double>` ↔ `list` / numpy 可转对象。

### 方案 D：Cython / nanobind（了解即可）

- **nanobind**：pybind11 作者的继任者，编译更快、体积更小，新项目可以选它，
  API 与 pybind11 几乎一致。
- **Cython**：适合"把现有 Python 循环加速"，而不是包装 C++ 类，
  定位与上面不同。

### 方案对比总结

| | subprocess | extern "C" + ctypes | pybind11 |
|---|---|---|---|
| C++ 端改动 | 无 | 加 `extern "C"` 接口层 | 加 bindings.cpp |
| 能用 C++ 类 | 否 | 否（只能函数） | 是 |
| 异常安全 | 天然隔离 | 需手动 catch | 自动转 Python 异常 |
| 额外依赖 | 无 | 无 | `pip install pybind11` |
| 适合 | 独立批量模拟 | 几个小函数 | 类的深度交互 |

---

## 5. C++ 项目结构与构建（本仓库规范）

### 5.1 目录布局

```
project/cpp/<snake_case 项目名>/
├── main.cpp                 # 入口：渲染循环 + 事件处理（唯一碰 SDL2 的地方）
├── include/
│   ├── Math/Vector.h        # 按类名 PascalCase，一个类一对 .h/.cpp
│   └── Physics/DoublePendulum.h
├── src/
│   ├── Math/Vector.cpp
│   └── Physics/DoublePendulum.cpp
├── document.md              # 物理模型、数值方法与运行方式（可选）
└── Makefile
```

### 5.2 项目内 Makefile 模板

全仓库 C / C++ 项目统一这个骨架（产物进 `build/`，已被 `.gitignore` 忽略）：

```makefile
CXX       ?= clang++
BUILD_DIR := build
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -g -Iinclude
CXXFLAGS  += $(shell pkg-config --cflags sdl2 SDL2_ttf)
LDLIBS    := $(shell pkg-config --libs sdl2 SDL2_ttf)

SRCS   := main.cpp $(wildcard src/*/*.cpp)
OBJS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS   := $(OBJS:.o=.d)
TARGET := $(BUILD_DIR)/<项目名>

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# -MMD -MP 自动生成头文件依赖，改 include/ 下的头文件也会触发重编
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
```

### 5.3 顶层调度

在仓库根目录：

```bash
make                            # 构建全部项目（C/C++ 编译，Python 语法检查）
make cpp                        # 只构建全部 C++ 项目
make run-cpp-double-pendulum    # 构建并运行某个项目
make clean-cpp                  # 只清理 C++ 项目
```

新增 C++ 项目的完整流程：`project/cpp/<名字>/` 建目录 → 放代码 + 上面模板的
Makefile（改 `TARGET`）→ 根 `Makefile` 的 `CPP_PROJECTS` 列表加一行 → 完成。
命名细则见 [docs/naming_conventions.md](../docs/naming_conventions.md)。

### 5.4 可视化选型：SDL2 还是 Python？

| 需求 | 选择 |
|------|------|
| 实时动画、交互按键（暂停 / 重置 / 拖尾） | C++ 端 SDL2（或 C 端 Raylib） |
| 论文图、批量出图、拟合标注 | Python matplotlib |
| 两者都要 | C++ 里跑模拟写数据，Python 再画静态图 |

实时渲染的代价是代码量：事件循环、帧率、坐标变换都要自己写。
所以本仓库的约定是 —— **教学演示 / 直观看现象用 SDL2，分析出图一律 Python**。

---

## 6. 工作流决策指南

### 最终决策速查

| 场景 | 方案 |
|------|------|
| 几百行以内的一次性小程序 | C 单文件 + Raylib（`project/c/` 模式） |
| 多物理对象 / 复用 / 扫参数 | C++ 类 + `include/` `src/` 布局（`project/cpp/` 模式） |
| 算完出论文图 | C/C++ 写 CSV → Python `loadtxt` + matplotlib |
| 实时交互演示 | C++ SDL2 / C Raylib，不走数据中转 |
| Jupyter 交互调参 + C++ 性能 | pybind11 绑定物理类 |
| 只想调几个函数、零依赖 | `extern "C"` + ctypes |
| 瓶颈只是 Python 循环 | 先试 numpy 向量化 / numba，别急着写 C++ |

### 流程图

```
要做什么？
│
├─ 验证算法 / 小演示 ──→ C 单文件（project/c/）──→ Raylib 实时看
│
├─ 认真做一个物理模型 ──→ C++ 类封装（project/cpp/）
│       │                        │
│       ├─ 要实时动画 ──→ SDL2 渲染层（main.cpp）
│       └─ 要分析出图 ──→ 写 CSV ──→ project/python/ matplotlib
│
└─ 交互式探索 / 扫参数 ──→ pybind11 把 C++ 类搬进 Jupyter
```

---

## 附录：常用命令速查

```bash
# ── 构建（仓库根目录）──
make                            # 全部项目
make run-cpp-double-pendulum    # 构建并运行
make -C project/cpp/double_pendulum   # 也可进入项目目录单独 make

# ── 编译 pybind11 模块（macOS）──
pip install pybind11
c++ -O2 -std=c++17 -shared -undefined dynamic_lookup \
    $(python3 -m pybind11 --includes) bindings.cpp \
    -o pendulum$(python3-config --extension-suffix)

# ── 编译 ctypes 动态库 ──
c++ -O2 -shared -fPIC -o libbridge.dylib bridge.cpp   # macOS
c++ -O2 -shared -fPIC -o libbridge.so    bridge.cpp   # Linux

# ── Python 环境定位 ──
python3 -m pybind11 --includes          # pybind11 头文件路径
python3-config --extension-suffix       # 本平台扩展名（macOS: .cpython-3xx-darwin.so）
python3 -c "import sysconfig; print(sysconfig.get_path('include'))"

# ── SDL2 依赖 ──
brew install sdl2 sdl2_ttf
pkg-config --cflags --libs sdl2 SDL2_ttf
```
