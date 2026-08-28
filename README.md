# c4phy

该仓库是我学习**计算物理**的记录。

---

## 目录

- [主要内容](#主要内容)
- [项目结构](#项目结构)
- [环境与运行](#环境与运行)
- [工作流](#工作流)
- [学习进度](#学习进度)
- [参考资料](#参考资料)
- [个人介绍](#个人介绍)

---

## 主要内容

本仓库包含两个方面：

1. **学习笔记** — 计算物理核心主题的整理笔记，配合 C 代码示例与 Python 可视化
2. **项目实践** — 独立的小型计算物理项目，C / C++ 负责数值计算，Python / Raylib / SDL2 负责可视化

目前还在不断完善中。

---

## 项目结构

```
c4phy/
├── notes/                              # 学习笔记
│   ├── error/                          #   误差分析（减法抵消、舍入误差、误差累积）
│   ├── Differentiation and Integration/#   数值微分与积分（向前/向后/中心差分，梯形/辛普森积分）
│   ├── Trial-and-Error Searching and Data Fitting/  # 试错搜索与数据拟合（二分/牛顿法，拉格朗日插值）
│   ├── Matrix Computing and N-D searching/  # 矩阵计算与 N 维搜索
│   ├── Monte Carlo Simulations/        #   蒙特卡洛模拟（随机数生成、随机行走、蛋白质折叠）
│   ├── Differential Equations and Nonlinear Oscillations/  # 微分方程与非线性振荡
│   ├── Fourier Analyses/               #   傅里叶分析
│   ├── C_Python_Workflow.md           #   C + Python 计算物理工作流手册
│   └── Project_Documentation_Template.md #   项目文档通用模板
│
├── project/
│   ├── c/                              # C 语言项目
│   │   ├── simple_harmonic_oscillation/  # 简谐振动
│   │   ├── damped_oscillation/           # 阻尼振动
│   │   ├── double_pendulum/              # 双摆（Raylib 实时可视化）
│   │   ├── random_walk/                  # 随机行走（Raylib 可视化，含自回避行走 / 蛋白质折叠）
│   │   ├── spontaneous_decay/            # 放射性自发衰变模拟
│   │   ├── ideal_gas/                    # 理想气体（Raylib 可视化）
│   │   └── reflection_of_light_in_a_sphere/  # 球内光线反射（Raylib 可视化）
│   ├── cpp/                             # C++ 项目（SDL2 可视化）
│   │   ├── simple_harmonic_oscillation/  # 简谐振动（相空间可视化）
│   │   └── double_pendulum/              # 双摆（RK4 + 轨迹拖尾 + 能量监测）
│   └── python/                          # Python 项目
│       ├── simple_harmonic_oscillation/  # 简谐振动可视化
│       ├── damped_oscillation/           # 阻尼振动可视化
│       ├── random_walk/                  # 随机行走可视化
│       ├── spontaneous_decay/            # 自发衰变可视化
│       └── ideal_gas/                    # 理想气体可视化
│
├── docs/
│   └── naming_conventions.md            # 项目结构与命名规则
└── README.md
```

> 目录与文件的详细命名规则见 [docs/naming_conventions.md](docs/naming_conventions.md)。

---

## 环境与运行

### 依赖

- **C / C++ 编译器**: GCC 或 Clang
- **Raylib**（部分 C 项目用于可视化）:
  ```bash
  brew install raylib
  ```
- **SDL2 / SDL2_ttf**（C++ 项目用于可视化）:
  ```bash
  brew install sdl2 sdl2_ttf
  ```
- **Python 3.x** + 科学计算栈:
  ```bash
  pip install numpy matplotlib
  ```

### 编译 & 运行示例

**C 项目（以双摆为例）：**

```bash
cd project/c/double_pendulum
gcc double_pendulum.c -o double_pendulum $(pkg-config --cflags --libs raylib)
./double_pendulum
```

**纯数值 C 项目（以阻尼振动为例）：**

```bash
cd project/c/damped_oscillation
gcc -O3 main.c -o main -lm
./main
```

**C++ 项目（Make 统一管理，在仓库根目录执行，产物输出到各项目 `build/`）：**

```bash
make                        # 一键编译全部 C++ 项目
make run-double-pendulum    # 编译并运行双摆: Space 暂停 / R 随机重置 / C 清除轨迹
make run-sho                # 编译并运行简谐振动
make clean                  # 清理全部 build/
```

也可进入单个项目目录（如 `project/cpp/double_pendulum`）执行 `make`、`make run`、`make clean`。

**Python 项目：**

```bash
python project/python/random_walk/random_walk.py
```

### 笔记中的代码

笔记目录下的 C 代码示例通常可直接编译运行：

```bash
cd notes/error
gcc ex1.c -o ex1 -lm && ./ex1
```

---

## 工作流

本仓库遵循 **C 做数值核心 + Python 做可视化** 的工作流。详细策略参考：

[📖 C + Python 计算物理工作流手册](notes/C_Python_Workflow.md)

核心思路：

```
数值计算 (C) → 数据写出 (CSV/二进制) → 可视化分析 (Python/matplotlib)
```

部分项目直接使用 Raylib / SDL2 在 C / C++ 端完成实时渲染（如双摆），省去数据中转。

---

## 学习进度

| 主题 | 笔记 | C 代码 | 可视化 |
|------|:----:|:------:|:------:|
| 误差分析（减法抵消、舍入误差、随机行走误差模型） | ✅ | ✅ | — |
| 数值微分（向前/向后/中心差分） | ✅ | ✅ | ✅ |
| 数值积分（梯形法、辛普森法） | ✅ | ✅ | — |
| 试错搜索与数据拟合（二分法、牛顿法、拉格朗日插值） | ✅ | ✅ | — |
| 矩阵计算与 N 维搜索 | ✅ | — | — |
| 蒙特卡洛模拟（随机数、随机行走、蛋白质折叠） | ✅ | ✅ | — |
| 傅里叶分析 | ✅ | — | — |
| 微分方程与非线性振荡 | ✅ | — | — |
| 简谐振动 | — | ✅ | ✅ |
|阻尼振动|-|✅|✅|
| 双摆 | ✅ | ✅ | ✅ |
| 随机行走（含自回避行走） | ✅ | ✅ | ✅ |
| 自发衰变 | — | ✅ | ✅ |
| 理想气体 | — | ✅ | ✅ |
| 球内光线反射 | — | ✅ | ✅ |

---

## 参考资料

本仓库笔记主要参考：

- *Computational Physics* — Rubin H. Landau, Manuel J. Páez, Cristian C. Bordeianu

---

## 个人介绍

我只是一个 NJU 物理系的学生。
