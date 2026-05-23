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
2. **项目实践** — 独立的小型计算物理项目，C 负责数值计算，Python / Raylib 负责可视化

目前还在不断完善中。

---

## 项目结构

```
c4phy/
├── notes/                              # 学习笔记
│   ├── error/                          #   误差分析（减法抵消、舍入误差、误差累积）
│   ├── Differentiation and Integration/#   数值微分与积分（向前/向后/中心差分，梯形/辛普森积分）
│   ├── Monte Carlo Simulations/        #   蒙特卡洛模拟（随机数生成、随机行走、蛋白质折叠）
│   └── C_Python_Workflow.md           #   C + Python 计算物理工作流手册
│
├── src/
│   ├── c/project/                      # C 语言项目
│   │   ├── simple harmonic oscillations/  # 简谐振动
│   │   ├── double pendulum/               # 双摆（Raylib 实时可视化）
│   │   ├── random walk/                   # 随机行走（含自回避行走 / 蛋白质折叠）
│   │   ├── Spontaneous decay/             # 放射性自发衰变模拟
│   │   └── Reflection of light in a sphere/  # 球内光线反射
│   └── python/project/                 # Python 项目
│       ├── random walk/                   # 随机行走可视化
│       └── spontaneous dacey/             # 自发衰变可视化
│
└── README.md
```

---

## 环境与运行

### 依赖

- **C 编译器**: GCC 或 Clang
- **Raylib**（部分项目用于可视化）:
  ```bash
  brew install raylib
  ```
- **Python 3.x** + 科学计算栈:
  ```bash
  pip install numpy matplotlib
  ```

### 编译 & 运行示例

**C 项目（以双摆为例）：**

```bash
cd "src/c/project/double pendulum"
gcc double_pendulum.c -o double_pendulum $(pkg-config --cflags --libs raylib)
./double_pendulum
```

**纯数值 C 项目（以随机行走为例）：**

```bash
cd "src/c/project/random walk"
gcc -O3 main.c -o main -lm
./main
```

**Python 项目：**

```bash
python "src/python/project/random walk/random walk.py"
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

部分项目直接使用 Raylib 在 C 端完成实时渲染（如双摆），省去数据中转。

---

## 学习进度

| 主题 | 笔记 | C 代码 | 可视化 |
|------|:----:|:------:|:------:|
| 误差分析（减法抵消、舍入误差、随机行走误差模型） | ✅ | ✅ | — |
| 数值微分（向前/向后/中心差分） | ✅ | ✅ | ✅ |
| 数值积分（梯形法、辛普森法） | ✅ | ✅ | — |
| 蒙特卡洛模拟（随机数、随机行走、蛋白质折叠） | ✅ | ✅ | — |
| 简谐振动 | — | ✅ | — |
| 双摆 | — | ✅ | ✅ |
| 随机行走（含自回避行走） | ✅ | ✅ | ✅ |
| 自发衰变 | — | ✅ | ✅ |
| 球内光线反射 | — | ✅ | — |

---

## 参考资料

本仓库笔记主要参考：

- *Computational Physics* — Rubin H. Landau, Manuel J. Páez, Cristian C. Bordeianu

---

## 个人介绍

我只是一个 NJU 物理系的学生。
