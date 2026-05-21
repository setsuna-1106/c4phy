# C + Python 计算物理工作流手册

> 面向场景：C 做数值计算核心，Python 做可视化和数据分析
> 最后更新：2026-05-19

---

## 目录

1. [C 绘图 vs Python 绘图对比](#1-c-绘图-vs-python-绘图对比)
2. [C 调用 Python 的 3 种方案](#2-c-调用-python-的-3-种方案)
3. [Python 调用 C 的 7 种方案](#3-python-调用-c-的-7-种方案)
4. [C 项目结构规范](#4-c-项目结构规范)
5. [工作流决策指南](#5-工作流决策指南)

---

## 1. C 绘图 vs Python 绘图对比

### 1.1 核心差异

| 维度 | C 绘图 | Python 绘图 |
|------|--------|-------------|
| 典型库 | gnuplot C API, PGPLOT, PLplot, Raylib, ImPlot, ROOT (C++) | matplotlib, seaborn, plotly, bokeh, PyQtGraph |
| 开发效率 | 低 — 每幅图大量样板代码 | 高 — 几行代码即可出图 |
| 交互性 | 需额外工作 | 开箱即用 (Jupyter, qt 后端) |
| 运行性能 | 极快 (编译型) | 较慢 (但绘图瓶颈在渲染，不在语言) |
| 论文输出 | 困难 | 直接 PDF/EPS + LaTeX 渲染 |
| 适合场景 | 嵌入式显示、实时渲染、与 C 数值代码紧耦合 | 探索性分析、论文出版级图表、快速原型 |

### 1.2 你的场景分析

#### 计算物理

```
数值计算 (C) → 数据写出 (HDF5/CSV/二进制) → 可视化分析 (Python)
```

- 计算物理的性能关键路径在数值积分、矩阵求解、Monte Carlo 采样——这些用 C
- 但绘图本身几乎从不是性能瓶颈
- Python 交互式环境（Jupyter）能调参数、换 colormap、加 annotation，改完立刻看到效果

#### 实验数据分析

- 错误条、拟合、统计标注 — matplotlib + scipy 一行代码
- 数据清洗 — pandas，C 不具备此生态
- 出版级输出 — matplotlib 支持 LaTeX 渲染，生成矢量 PDF/EPS

### 1.3 什么时候用 C 绘图？

1. **实时数据采集与显示** — DAQ 系统延迟 <10ms → ImPlot / Raylib / ROOT
2. **嵌入式 / 无 Python 环境** — FPGA+ARM 实验控制台
3. **大规模点云实时交互** — 百万级散点 rotate/zoom → C++ VTK / OpenGL

### 1.4 工具推荐

| 需求 | 推荐 | 理由 |
|------|------|------|
| 日常论文图 | **matplotlib** + seaborn | 生态最成熟 |
| 交互探索 | **plotly** | 鼠标悬停看数据点值 |
| 大数据集 (>1M 点) | **PyQtGraph** 或 **datashader** | 比 matplotlib 快 10-100 倍 |
| 高能物理 | **ROOT** (C++/Python 双接口) | CERN 标准 |
| 实时显示 | **ImPlot** (C++) 或 **PyQtGraph** (Python) | 低延迟刷新 |
| 3D 体渲染 | **VTK** 或 **VisIt** | 工业级 |

---

## 2. C 调用 Python 的 3 种方案

### 方案 A：`popen()` 管道传数据（最推荐）

**适合**：C 算完一批数据，Python 直接画图。99% 的场景用这个。

#### C 端

```c
#include <stdio.h>
#include <math.h>

int main() {
    /* 启动 Python 绘图脚本，打开管道 */
    FILE *py = popen("python3 plot.py", "w");
    if (!py) { perror("popen"); return 1; }

    /* 把计算数据写到 Python 的 stdin */
    double x, y;
    for (int i = 0; i <= 1000; i++) {
        x = i * 0.01;
        y = sin(x) * exp(-x * 0.3);
        fprintf(py, "%.6f %.6f\n", x, y);
    }

    /* 关管道 → Python 收到 EOF → 开始画图 */
    int ret = pclose(py);
    printf("Python exit: %d\n", ret);
    return 0;
}
```

编译：`gcc -O3 -o sim main.c -lm && ./sim`

#### Python 端 (`plot.py`)

```python
import sys
import numpy as np
import matplotlib.pyplot as plt

# 直接读 C 传过来的 stdin
data = np.loadtxt(sys.stdin)
x, y = data[:, 0], data[:, 1]

plt.plot(x, y, 'b-', lw=1.5)
plt.xlabel("x")
plt.ylabel("y")
plt.title("Damped oscillation (from C)")
plt.savefig("output.pdf", dpi=150)
plt.show()
```

#### 进阶：管道流式读取（大文件/实时显示）

```python
import sys
import numpy as np
import matplotlib.pyplot as plt
from collections import deque

x_buf, y_buf = deque(), deque()
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) == 2:
        x_buf.append(float(parts[0]))
        y_buf.append(float(parts[1]))
    # 每 10000 点刷新一次图（可选）
    if len(x_buf) >= 10000:
        # 更新图...
        x_buf.clear(); y_buf.clear()
```

### 方案 B：C 写文件 → `system()` 调用 Python

**适合**：数据量大（GB 级），二进制格式。

#### C 端

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX 500
#define NY 500

void write_field(const char *fname, double *u) {
    FILE *f = fopen(fname, "wb");
    fwrite(&(int){NX}, sizeof(int), 1, f);
    fwrite(&(int){NY}, sizeof(int), 1, f);
    fwrite(u, sizeof(double), NX * NY, f);
    fclose(f);
}

int main() {
    double *u = malloc(NX * NY * sizeof(double));

    /* 计算：二维热传导稳态解 */
    for (int i = 0; i < NX; i++)
        for (int j = 0; j < NY; j++)
            u[i * NY + j] = exp(-0.5 * ((i - NX/2.0)*(i - NX/2.0)
                                       + (j - NY/2.0)*(j - NY/2.0)) / 2500.0);

    write_field("field.bin", u);
    free(u);

    /* 调用 Python 画图 */
    system("python3 plot_2d.py field.bin");
    return 0;
}
```

#### Python 端 (`plot_2d.py`)

```python
import sys
import numpy as np
import matplotlib.pyplot as plt

with open(sys.argv[1], 'rb') as f:
    nx = int.from_bytes(f.read(4), 'little')
    ny = int.from_bytes(f.read(4), 'little')
    u = np.frombuffer(f.read(), dtype=np.float64).reshape(nx, ny)

plt.imshow(u, cmap='inferno', origin='lower', aspect='auto')
plt.colorbar(label='u(x,y)')
plt.savefig('field.png', dpi=200)
```

### 方案 C：嵌入 Python 解释器（Python C API）

**适合**：C 和 Python 需要紧密交互，C 调用 Python 函数并获取返回值。

```c
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>

int main() {
    Py_Initialize();

    /* 添加当前目录到 Python 搜索路径 */
    PyRun_SimpleString("import sys; sys.path.insert(0, '.')");

    /* 构建数据并传给 Python */
    int n = 1000;
    PyObject *pList = PyList_New(n);
    for (int i = 0; i < n; i++) {
        double x = i * 0.01;
        double y = sin(x) * exp(-x * 0.3);
        PyObject *pTuple = PyTuple_New(2);
        PyTuple_SetItem(pTuple, 0, PyFloat_FromDouble(x));
        PyTuple_SetItem(pTuple, 1, PyFloat_FromDouble(y));
        PyList_SetItem(pList, i, pTuple);
    }

    /* 注入到 Python 全局变量 */
    PyObject *pMain = PyImport_AddModule("__main__");
    PyObject *pDict = PyModule_GetDict(pMain);
    PyDict_SetItemString(pDict, "data", pList);
    Py_DECREF(pList);

    /* 在嵌入的解释器里运行 matplotlib */
    PyRun_SimpleString(
        "import matplotlib\n"
        "matplotlib.use('Agg')\n"
        "import matplotlib.pyplot as plt\n"
        "import numpy as np\n"
        "arr = np.array(data)\n"
        "plt.plot(arr[:,0], arr[:,1])\n"
        "plt.xlabel('x'); plt.ylabel('y')\n"
        "plt.title('C Embedded → Python Plot')\n"
        "plt.savefig('embedded.png', dpi=150)\n"
        "print('Saved embedded.png')\n"
    );

    Py_Finalize();
    return 0;
}
```

编译（macOS）：
```bash
gcc -O3 -o embed embed.c \
    -I$(python3 -c "import sysconfig; print(sysconfig.get_path('include'))") \
    -L$(python3 -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))") \
    -lpython3.12 -lm
```

> **注意**：Python C API 是最高级但也最复杂的方式。对计算物理，**方案 A（popen）在 90% 的场景下就够用了**。

### 方案对比总结

| | popen 管道 | system + 文件 | Python C API |
|---|---|---|---|
| C 代码改动 | 最小（`fprintf`） | 最小 | 很大 |
| Python 代码 | 现成脚本不改 | 现成脚本不改 | 嵌在 C 字符串 |
| 数据量 | 小/中（MB 级） | 大（GB 级二进制） | 不限 |
| 学习成本 | 5 分钟 | 5 分钟 | 半天 |
| 返回结果 | exit code | 文件 | 任意 Python 对象 |

---

## 3. Python 调用 C 的 7 种方案

> 这部分是反方向：Python 作为主程序，调用 C 函数库。

### 决策树

```
你的 Python 代码哪里慢？
│
├─ 内层循环（粒子力、网格更新）
│   ├─ 已经是 numpy 向量化？→ 试试 numba @njit
│   └─ numba 还不够？→ Cython 或 pybind11
│
├─ 有现成 C/C++ 物理库
│   ├─ 只需调用几个函数 → ctypes / cffi
│   ├─ 需要面向对象交互 → pybind11
│   └─ CERN ROOT 生态 → 自带 PyROOT
│
├─ C 和 Python 完全解耦运行
│   └─ HDF5（大型模拟）或 subprocess pipe（轻量）
│
└─ 其实不慢？→ 继续用 numpy + scipy
```

### 3.1 subprocess pipe（零依赖）

```python
import subprocess
import numpy as np

result = subprocess.run(
    ["./solver"], capture_output=True, text=True
)
data = np.loadtxt(result.stdout.splitlines())
```

### 3.2 ctypes（Python 标准库自带）

#### C 端编译

```bash
# macOS
gcc -O3 -shared -fPIC -o libphysics.dylib physics.c -lm
# Linux
gcc -O3 -shared -fPIC -o libphysics.so physics.c -lm
# Windows
gcc -O3 -shared -o physics.dll physics.c -lm
```

#### C 代码 (`physics.c`)

```c
#include <math.h>

void heat1d_step(double *u, const double *u_prev, int n,
                 double alpha, double dt, double dx) {
    double r = alpha * dt / (dx * dx);
    for (int i = 1; i < n - 1; i++) {
        u[i] = u_prev[i] + r * (u_prev[i+1] - 2*u_prev[i] + u_prev[i-1]);
    }
}

void lorentz_residuals(const double *params, const double *x,
                       const double *y_data, double *residuals, int n) {
    double A = params[0], x0 = params[1], gamma = params[2], offset = params[3];
    for (int i = 0; i < n; i++) {
        double dx = (x[i] - x0) / gamma;
        residuals[i] = y_data[i] - (A / (1.0 + dx*dx) + offset);
    }
}
```

#### Python 桥接层 (`bridge.py`)

```python
import ctypes
import numpy as np
import os

_lib = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "libphysics.dylib"))

# 声明函数签名
_lib.heat1d_step.argtypes = [
    ctypes.POINTER(ctypes.c_double),   # u (输出)
    ctypes.POINTER(ctypes.c_double),   # u_prev
    ctypes.c_int,
    ctypes.c_double, ctypes.c_double, ctypes.c_double,
]
_lib.heat1d_step.restype = None

_lib.lorentz_residuals.argtypes = [
    ctypes.POINTER(ctypes.c_double),   # params[4]
    ctypes.POINTER(ctypes.c_double),   # x[]
    ctypes.POINTER(ctypes.c_double),   # y_data[]
    ctypes.POINTER(ctypes.c_double),   # residuals[]
    ctypes.c_int,
]
_lib.lorentz_residuals.restype = None

# 包装函数
def heat1d_step(u, u_prev, alpha, dt, dx):
    """C 热传导单步，传入 numpy 数组"""
    _lib.heat1d_step(
        u.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        u_prev.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        len(u), alpha, dt, dx
    )

def lorentz_residuals(params, x, y_data):
    """返回残差，用于 scipy.optimize.least_squares"""
    n = len(x)
    res = np.empty(n, dtype=np.float64)
    _lib.lorentz_residuals(
        np.ascontiguousarray(params, dtype=np.float64).ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        np.ascontiguousarray(x, dtype=np.float64).ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        np.ascontiguousarray(y_data, dtype=np.float64).ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        res.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        n,
    )
    return res
```

#### 使用示例

```python
import numpy as np
import matplotlib.pyplot as plt
from bridge import heat1d_step, lorentz_residuals
from scipy.optimize import least_squares

# 热传导模拟
n = 500
u = np.zeros(n)
u_prev = np.zeros(n)
u_prev[220:280] = 1.0

for step in range(2000):
    heat1d_step(u, u_prev, 0.01, 0.001, 1.0/n)
    u, u_prev = u_prev, u

# Lorentz 拟合
result = least_squares(
    lambda p: lorentz_residuals(p, x_data, y_data),
    x0=[1000, 10.0, 0.1, 0.0]
)
```

### 3.3 ctypes 类型映射速查

| C type | ctypes type | numpy |
|--------|-------------|-------|
| `double` | `ctypes.c_double` | `np.float64` |
| `int` | `ctypes.c_int` | `np.int32` |
| `double *` | `POINTER(c_double)` | `arr.ctypes.data_as(...)` |
| `const double *` | `POINTER(c_double)` | 同上 |
| `void` | `None` (restype) | — |

### 3.4 pybind11（最深度集成）

仅适用 C++，给 Python 暴露完整的类和对象模型。详见 pybind11 官方文档。

### 3.5 Cython（Python → C 加速）

适合已有 Python 数值代码，只需加速瓶颈循环。

### 3.6 numba（JIT 编译，零 C 代码）

```python
from numba import njit

@njit
def lj_force(x, y, z, sigma, epsilon):
    n = len(x)
    fx = np.zeros(n)
    # ... C 速度的纯 Python 循环
    return fx
```

### 3.7 HDF5 文件桥接

C 写 HDF5 (`hdf5.h`)，Python 读 (`h5py`) — 计算物理的标准大数据方案。

---

## 4. C 项目结构规范

### 层级 1：单文件原型

```
project/
└── main.c
```

`< 500 行`，快速验证算法。

### 层级 2：头文件拆分

```
project/
├── main.c
├── physics.h
├── physics.c
├── io.h
├── io.c
└── Makefile
```

`500-3000 行`，函数开始分类。

### 层级 3：计算物理标准布局（推荐）

```
project/
├── src/                  # 所有 .c 源文件
│   ├── main.c            # 入口
│   ├── integrator.c      # 数值积分 (RK4, Verlet, ...)
│   ├── force.c           # 力计算 (LJ, Coulomb, ...)
│   ├── potential.c       # 势函数
│   ├── random.c          # 随机数生成器
│   ├── output.c          # 写 HDF5 / CSV / 二进制
│   └── parser.c          # 读输入参数文件
│
├── include/              # 所有 .h 头文件
│   ├── integrator.h
│   ├── force.h
│   ├── potential.h
│   ├── random.h
│   ├── output.h
│   └── parser.h
│
├── test/                 # 单元测试
│   ├── test_integrator.c
│   ├── test_force.c
│   └── test_potential.c
│
├── data/                 # 输入数据（不上 git）
├── output/               # 输出目录
├── scripts/              # Python 后处理
│   ├── plot.py
│   └── analyze.py
│
├── Makefile
└── README.md
```

#### Makefile 模板

```makefile
CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -march=native -fopenmp
LDFLAGS = -lm
INCDIR  = include
SRCDIR  = src
OBJDIR  = build

SRCS    = $(wildcard $(SRCDIR)/*.c)
OBJS    = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
TARGET  = sim

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

test: $(TARGET)
	./$(TARGET) --test

.PHONY: clean test
```

### 层级 4：库 + 应用分离

```
project/
├── libphys/              # 可复用的物理库
│   ├── include/phys/
│   ├── src/
│   └── Makefile           # → libphys.a
│
├── app/                   # 应用程序
│   ├── src/main.c
│   └── Makefile           # 链接 libphys
│
├── scripts/               # Python 后处理
└── Makefile
```

---

## 5. 工作流决策指南

### C + Python 融合项目推荐结构

```
project/
├── src/              # C 数值核心
├── include/
├── libphysics.dylib  # ctypes 调用目标
├── scripts/          # Python 绘图/分析
│   ├── bridge.py     # ctypes 桥接层
│   └── plot.py       # 实际画图
├── Makefile
└── run.sh            # 一键: make && python3 scripts/plot.py
```

`run.sh`:
```bash
#!/bin/bash
make -j$(sysctl -n hw.ncpu 2>/dev/null || nproc) && python3 scripts/plot.py
```

### 最终决策速查

| 场景 | 方案 | 命令/代码 |
|------|------|----------|
| C 主程序，算完画图 | **C → popen → Python** | `FILE *py = popen("python3 plot.py", "w");` |
| Python 主程序，调 C 函数 | **ctypes** | `gcc -shared -fPIC` + `ctypes.CDLL` |
| C 和 Python 各自独立 | **HDF5 文件桥接** | C 写 `hdf5.h`，Python 读 `h5py` |
| 纯 Python 太慢 | **numba @njit** | 不改 C 一行 |
| Python 调 C++ 类 | **pybind11** | 需 C++ 编译器 |
| 大量实时交互 | **PyQtGraph** / **ImPlot** | 延迟 <10ms |

### 流程图

![工作流流程图](c_python_workflow.png)

---

## 附录：常用命令速查

```bash
# C 编译
gcc -O3 -Wall -o prog main.c -lm                            # 单文件
gcc -O3 -shared -fPIC -o libphys.dylib phys.c -lm           # 动态库 (macOS)
gcc -O3 -shared -fPIC -o libphys.so phys.c -lm              # 动态库 (Linux)
gcc -O3 -c physics.c -o physics.o                           # 目标文件
ar rcs libphys.a physics.o integrator.o                     # 静态库

# Python 相关
python3 -c "import sysconfig; print(sysconfig.get_path('include'))"  # Python 头文件路径
python3 -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))"  # Python 库路径
python3-config --includes                                    # 编译选项
python3-config --ldflags                                     # 链接选项
```
