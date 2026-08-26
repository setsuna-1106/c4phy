#include "Physics/DoublePendulum.h"

// Euler-Lagrange 方程导出的双摆角加速度(轻杆、无阻尼)
DPState DoublePendulum::deriv(const DPState& s) const {
    const double d  = s.th1 - s.th2;
    const double dd = 2 * p_.m1 + p_.m2 - p_.m2 * std::cos(2 * d);

    DPState r;
    r.th1 = s.w1;
    r.th2 = s.w2;
    r.w1 = (-p_.g * (2 * p_.m1 + p_.m2) * std::sin(s.th1)
            - p_.m2 * p_.g * std::sin(s.th1 - 2 * s.th2)
            - 2 * std::sin(d) * p_.m2 * (s.w2 * s.w2 * p_.l2
                                          + s.w1 * s.w1 * p_.l1 * std::cos(d)))
           / (p_.l1 * dd);
    r.w2 = (2 * std::sin(d)
            * (s.w1 * s.w1 * p_.l1 * (p_.m1 + p_.m2)
               + p_.g * (p_.m1 + p_.m2) * std::cos(s.th1)
               + s.w2 * s.w2 * p_.l2 * p_.m2 * std::cos(d)))
           / (p_.l2 * dd);
    return r;
}

DoublePendulum::DoublePendulum(const DPParams& params, const DPState& state)
    : p_(params), t_(0) {
    reset(state);
}

void DoublePendulum::reset(const DPState& state) {
    s_ = state;
    e0_ = energy();
    t_ = 0;
}

void DoublePendulum::step(double dt) {
    const DPState k1 = deriv(s_);
    const DPState k2 = deriv(s_ + 0.5 * dt * k1);
    const DPState k3 = deriv(s_ + 0.5 * dt * k2);
    const DPState k4 = deriv(s_ + dt * k3);
    s_ = s_ + dt / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    t_ += dt;
}

double DoublePendulum::energy() const {
    const double d = s_.th1 - s_.th2;
    return 0.5 * (p_.m1 + p_.m2) * p_.l1 * p_.l1 * s_.w1 * s_.w1
         + 0.5 * p_.m2 * p_.l2 * p_.l2 * s_.w2 * s_.w2
         + p_.m2 * p_.l1 * p_.l2 * s_.w1 * s_.w2 * std::cos(d)
         - (p_.m1 + p_.m2) * p_.g * p_.l1 * std::cos(s_.th1)
         - p_.m2 * p_.g * p_.l2 * std::cos(s_.th2);
}

double DoublePendulum::energy_error() const {
    return (energy() - e0_) / e0_;
}
