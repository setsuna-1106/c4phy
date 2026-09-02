# 阻尼受迫单摆

使用 C 语言数值求解受正弦驱动力作用的阻尼单摆，在 Raylib 窗口中实时演示摆动动画、$\theta$-$t$ 时间序列与 $\theta$-$\omega$ 相图，同时输出数据文件，与小角度线性理论的解析稳态解对比验证。

---

## 目录

1. [题目描述](#1-题目描述)
2. [物理模型](#2-物理模型)
3. [数值方法](#3-数值方法)
4. [实现细节 / 代码结构](#4-实现细节--代码结构)
5. [编译 & 运行](#5-编译--运行)
6. [验证 & 结果](#6-验证--结果)
7. [参考资料](#7-参考资料)

---

## 1. 题目描述

模拟一个受重力、线性阻尼和正弦变化驱动力矩共同作用的单摆。随机初始化角位移与角速度后积分运动方程，观察系统在瞬态衰减后进入与驱动力同频的周期稳态，输出 $\theta$-$t$ 时间序列与 $\theta$-$\omega$ 相图数据。

## 2. 物理模型

### 自由度

| 变量 | 符号 | 含义 |
|------|------|------|
| 角位移 | $\theta$ | 摆与竖直向下方向的夹角，逆时针为正 |
| 角速度 | $\omega$ | $\dot\theta$ |

### 控制方程

$$
\ddot\theta = -\omega_0^2 \sin\theta - \Gamma\dot\theta + F\sin(\Omega t)
$$

化为二维一阶方程组 $y = (\theta, \omega)$：

$$
\dot\theta = \omega, \qquad \dot\omega = -\omega_0^2 \sin\theta - \Gamma\omega + F\sin(\Omega t)
$$

### 参数表

| 参数 | 符号 | 值 | 单位 |
|------|------|----|------|
| 阻尼系数 | $\Gamma$ | 1 | s⁻¹ |
| 固有角频率（小角度） | $\omega_0$ | 1 | rad/s |
| 驱动力角频率 | $\Omega$ | 1 | rad/s |
| 驱动力幅值 | $F$ | 1 | rad/s² |

驱动频率恰好等于小角度固有频率（$\Omega = \omega_0$，共振情形），线性阻尼较弱（$\Gamma < 2\omega_0$，欠阻尼）。

### 初始条件

| 变量 | 符号 | 初始值 | 含义 |
|------|------|--------|------|
| 角位移 | $\theta_0$ | $[-\pi, \pi)$ 均匀随机 | 随机数生成 |
| 角速度 | $\omega_0^{init}$ | $[-5, 5)$ 均匀随机 | 随机数生成，种子取当前时间 |

### 假设 / 近似

- 阻尼力矩与角速度成正比（线性阻尼）
- 驱动力矩为纯正弦，无高次谐波
- 摆为理想单摆（轻杆 + 质点），重力加速度已并入 $\omega_0^2 = g/L$
- 输出前将 $\theta$ 卷绕到 $(-\pi, \pi]$（角度为循环变量，相图的标准画法）

### 守恒量

| 守恒量 | 公式 | 备注 |
|--------|------|------|
| 无 | — | 阻尼耗散机械能、驱动力注入能量，总能量随时间变化，不存在守恒量；验证改用稳态解析解对比（见第 6 节） |

## 3. 数值方法

### 算法

- **算法名称**: 经典四阶 Runge-Kutta（RK4）

**递推格式**（对二维状态 $y$，右端函数 $f(t, y)$）：

```
k1 = f(t, y)
k2 = f(t + dt/2, y + dt/2 * k1)
k3 = f(t + dt/2, y + dt/2 * k2)
k4 = f(t + dt,   y + dt * k3)
y_next = y + dt/6 * (k1 + 2*k2 + 2*k3 + k4)
```

### 时间步长

- **dt**: 0.01。系统最快时间尺度为驱动周期 $2\pi/\Omega \approx 6.28$，每周期约 628 步；dt 减半后稳态振幅仅变化 $8\times10^{-6}$ rad（见第 6 节），步长充分收敛。

### 终止条件

- 固定步数 `N = 10000`，积分到 $t = 100$（约 16 个驱动周期，远长于瞬态衰减时间尺度 $2/\Gamma = 2$ s）。

### 已知局限

- 大驱动力（$F \gtrsim 2$）时系统进入混沌，长时间轨迹对初值敏感，浮点精度下不可复现精确轨迹（但统计性质稳定）
- 相图按 $(-\pi, \pi]$ 卷绕，$\theta$-$t$ 时间序列在瞬态大幅转动段会出现 $\pm 2\pi$ 的跳变

## 4. 实现细节 / 代码结构

### 文件列表

| 文件 | 职责 |
|------|------|
| `main.c` | 物理参数、右端函数、随机初始化、主循环、CSV 输出与 Raylib 实时可视化 |
| `rk4.c` / `rk4.h` | 通用的二维一阶 ODE 组四阶 Runge-Kutta 步进模块（与具体物理无关，可复用） |

### 核心函数

| 函数 | 签名 | 职责 |
|------|------|------|
| `deriv()` | `void deriv(double t, double y[2], double dydt[2])` | 受迫阻尼摆的右端函数 |
| `init()` | `void init(void)` | 随机生成初始 $\theta \in [-\pi,\pi)$、$\omega \in [-5,5)$，并清空可视化轨迹 |
| `step()` | `void step(double t1)` | 调用 `rk4()` 推进一步，并把 $\theta$ 卷绕到 $(-\pi, \pi]$ |
| `rk4()` | `void rk4(deriv2 f, double y[2], double t, double dt)` | 经典 RK4 单步，就地更新 `y` |
| `push_trail()` | `void push_trail(void)` | 把当前 $(\theta,\omega)$ 追加到相图轨迹缓冲（超容量丢弃最老点） |
| `record_trace()` | `void record_trace(void)` | 把当前 $(t,\theta)$ 追加到时间序列缓冲（超容量丢弃最老样本） |
| `phase_pos()` / `trace_pos()` | `Vector2 phase_pos(double th, double om)` 等 | 物理坐标到相图 / 时间序列面板像素的映射 |
| `draw_pendulum()` / `draw_phase()` / `draw_trace()` | `void draw_xxx(void)` | 绘制左侧摆动画、右上相图、右下时间序列 |

### 输入 / 输出

- **输入**: 无。所有参数以宏常量硬编码，初始条件随机生成。
- **输出**: `theta-t.csv`（表头 `t,theta`，时间序列）与 `theta-omega.csv`（表头 `theta,omega`，相空间轨迹），由 `project/python/perturbed_damped_pendulum/main.py` 读取绘图。程序运行时同时实时可视化；前 `N` 步（$t \le 100$）的数据写入 CSV，之后动画继续、输出停止。

### 关键常量

| 宏 / 常量 | 值 | 含义 |
|-----------|----|------|
| `Gamma` | 1 | 阻尼系数 [s⁻¹] |
| `omega0` | 1 | 固有角频率 [rad/s] |
| `Omega` | 1 | 驱动力角频率 [rad/s] |
| `F` | 1 | 驱动力幅值 [rad/s²] |
| `dt` | 1e-2 | 时间步长 [s] |
| `N` | 10000 | 总步数 |
| `WIDTH` / `HEIGHT` | 960 / 600 | 窗口尺寸 [px] |
| `FPS` | 60 | 目标帧率 |
| `SPEED` | 4 | 每帧推进的物理步数（约 2.4 倍实时） |
| `ROD` | 180 | 摆长像素长度 [px] |
| `PHASE_TRAIL` | 3000 | 相图轨迹长度 [帧] |
| `TRACE_N` / `TRACE_EVERY` | 1200 / 4 | θ-t 采样数 / 每 4 步采 1 个样本（窗口 48 s） |

## 5. 编译 & 运行

### 依赖

- GCC / Clang
- Raylib（实时可视化）:
  ```bash
  brew install raylib
  ```
- Python 3.x + numpy + matplotlib（可选，用于复绘 CSV）

### 编译

```bash
# 或直接 make（推荐）
gcc -O2 main.c rk4.c -o main $(pkg-config --cflags --libs raylib)
```

### 运行

```bash
./main      # 打开实时可视化窗口，同时在项目目录下生成 theta-t.csv 与 theta-omega.csv
```

### 可视化说明

| 区域 / 按键 | 内容 |
|-------------|------|
| 左侧 | 摆的实时动画，摆球处的水平短线指示驱动力矩的方向与强弱 |
| 右上 | $\theta$-$\omega$ 相图，轨迹渐隐（越新越亮），红色点为当前状态 |
| 右下 | 最近 48 s 的 $\theta$-$t$ 时间序列，红色点为当前值 |
| `Space` | 暂停 / 继续 |
| `R` | 随机重置初始条件并清空轨迹 |
| `C` | 仅清空轨迹 |

绘图（可选）：

```bash
python3 ../../python/perturbed_damped_pendulum/main.py
```

## 6. 验证 & 结果

### 验证方式

- [x] **解析解对比**: 瞬态衰减后与小角度线性理论的稳态解对比
- [x] **收敛性测试**: dt 减半 → 稳态振幅变化
- [x] **定性合理性**: 稳态周期锁定驱动周期

线性理论（$\sin\theta \to \theta$）的稳态解为与驱动同频的正弦振动，振幅与相位滞后：

$$
A = \frac{F}{\sqrt{(\omega_0^2 - \Omega^2)^2 + (\Gamma\Omega)^2}}, \qquad
\tan\varphi = \frac{\Gamma\Omega}{\omega_0^2 - \Omega^2}
$$

本参数下 $\Omega = \omega_0$（共振），$A = F/(\Gamma\Omega) = 1$ rad，$\varphi = \pi/2$。

### 结果

实测（$t \ge 40$ 的稳态段，随机初值）：

| 量 | 数值 | 理论值 | 偏差 |
|----|------|--------|------|
| 稳态振幅 | 0.9889 rad | 1.0000 rad | 1.1% |
| 稳态周期 | 6.2833 | $2\pi/\Omega = 6.2832$ | 0.002% |

振幅比线性理论略小，方向与 $\sin\theta < \theta$ 的非线性软化一致：有效恢复力减弱使系统略偏离共振，按 $\omega_{\rm eff}^2 \approx \omega_0^2 \sin A / A \approx 0.83$ 修正后预期 $A \approx 0.99$ rad，与实测吻合。周期精确锁定驱动周期，相位滞后 $\pi/2$（时间序列峰值滞后驱动力峰值约 $2\pi/\Omega \times 1/4$）。

收敛性：dt 从 0.01 减半到 0.005（N 加倍保持总时间不变），稳态振幅变化 $8\times10^{-6}$ rad，步长已充分收敛。

瞬态行为：初始随机大角速度（$\omega_0^{init}$ 最大 ±5 rad/s）引起的转动在阻尼时间尺度（$\sim 2/\Gamma = 2$ s）内衰减，$t \approx 10$ 后系统已进入稳态极限环；相图 `theta-omega.csv` 中表现为收敛到闭合椭圆附近的轨迹环。

时间序列图与相图由 `project/python/perturbed_damped_pendulum/main.py` 生成（左：$t$-$\theta$；右：$\theta$-$\omega$ 相图），按仓库惯例不提交截图。

## 7. 参考资料

- N. J. Giordano, H. Nakanishi, *Computational Physics*, 2nd ed., Ch. 3（受迫阻尼摆与混沌的经典数值处理）
- 《理论力学》单摆受迫振动章节（线性稳态解 $A$、$\varphi$ 公式）
