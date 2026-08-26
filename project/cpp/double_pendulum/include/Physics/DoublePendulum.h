#pragma once

#include <cmath>

#include "Math/Vector.h"

// 双摆物理模型(纯数值,不依赖任何渲染库)
// 角度约定:th1、th2 从竖直向下方向量起,逆时针为正
// 坐标约定:悬挂点为原点,y 轴向上,单位制 SI(m / kg / s)

struct DPParams {
    double l1, l2;   // 摆长 [m]
    double m1, m2;   // 摆锤质量 [kg]
    double g;        // 重力加速度 [m/s^2]
};

struct DPState {
    double th1, th2; // 角度 [rad]
    double w1, w2;   // 角速度 [rad/s]
};

// 状态空间上的线性组合,RK4 需要的向量运算
inline DPState operator+(const DPState& a, const DPState& b) {
    return {a.th1 + b.th1, a.th2 + b.th2, a.w1 + b.w1, a.w2 + b.w2};
}

inline DPState operator*(double k, const DPState& a) {
    return {k * a.th1, k * a.th2, k * a.w1, k * a.w2};
}

class DoublePendulum {
public:
    DoublePendulum(const DPParams& params, const DPState& state);

    void step(double dt);              // RK4 推进一个定步长
    void reset(const DPState& state);  // 重置状态,初始能量与模拟时间归零

    const DPParams& params() const { return p_; }
    const DPState& state() const { return s_; }
    double time() const { return t_; }

    double energy() const;             // 当前总机械能 [J]
    double energy_error() const;       // 相对初始能量的漂移 (E - E0) / E0

    // 摆锤位置(物理坐标,悬挂点为原点,y 向上)[m]
    vector2D bob1() const {
        return {p_.l1 * std::sin(s_.th1), -p_.l1 * std::cos(s_.th1)};
    }

    vector2D bob2() const {
        return bob1() + vector2D{p_.l2 * std::sin(s_.th2),
                                 -p_.l2 * std::cos(s_.th2)};
    }

private:
    DPState deriv(const DPState& s) const;  // ODE 右端项:(th, w) -> (w, alpha)

    DPParams p_;
    DPState s_;
    double e0_;  // 初始能量,作为守恒量基准
    double t_;   // 累计模拟时间 [s]
};
