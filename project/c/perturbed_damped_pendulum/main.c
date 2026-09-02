#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rk4.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define Gamma 1  /*阻尼系数*/
#define omega0 1 /*固有角频率*/
#define Omega 1  /*驱动力角频率*/
#define F 1      /*驱动力幅值*/
#define dt 1e-2  /*时间步长*/
#define N 10000  /*总步数, 模拟到t=100*/

/*可视化参数*/
#define WIDTH 960
#define HEIGHT 600
#define FPS 60
#define SPEED 4          /*每帧推进的物理步数, 约为实时的2.4倍*/
#define ROD 180          /*摆长[像素]*/
#define BOB_RADIUS 12    /*摆球半径[像素]*/
#define LINE_THICK 2     /*摆杆线宽[像素]*/
#define PHASE_TRAIL 3000 /*相图轨迹长度[帧]*/
#define TRACE_N 1200     /*θ-t时间序列样本数*/
#define TRACE_EVERY 4    /*每多少物理步记录一个θ-t样本*/
#define T_WINDOW (TRACE_N * TRACE_EVERY * dt)      /*时间序列窗口长度[s]*/
#define PHASE_RECT (Rectangle){560, 60, 360, 220}  /*右上: θ-ω相图面板*/
#define TRACE_RECT (Rectangle){560, 360, 360, 200} /*右下: θ-t面板*/

double theta;
double omega;
double t;

/*可视化状态*/
bool paused;
Vector2 trail[PHASE_TRAIL]; /*相图轨迹(θ, ω)*/
int phase_len;
double trace_t[TRACE_N]; /*θ-t采样缓冲*/
double trace_theta[TRACE_N];
int trace_len;
int steps; /*已完成的物理步数*/

/*受迫阻尼摆的右端函数: y = (θ, ω)*/
void deriv(double t, double y[2], double dydt[2]) {
    dydt[0] = y[1];
    dydt[1] = -omega0 * omega0 * sin(y[0]) - Gamma * y[1] + F * sin(Omega * t);
}

/*随机生成初始角位移与角速度, 并清空可视化轨迹*/
void init() {
    theta = 2.0 * M_PI * rand() / RAND_MAX - M_PI; /*θ ∈ [-π, π)*/
    omega = 10.0 * rand() / RAND_MAX - 5.0;        /*ω ∈ [-5, 5)*/
    t = 0;
    phase_len = 0;
    trace_len = 0;
}

/*用RK4推进一步, 并把θ卷绕到(-π, π]以便绘制相图*/
void step(double t1) {
    double y[2] = {theta, omega};

    rk4(deriv, y, t, t1);
    omega = y[1];
    theta = fmod(y[0] + M_PI, 2.0 * M_PI);
    if (theta < 0) theta += 2.0 * M_PI;
    theta -= M_PI;
    t += t1;
}

/*把当前(θ, ω)追加到相图轨迹, 超出容量则丢弃最老的点*/
void push_trail() {
    if (phase_len < PHASE_TRAIL) phase_len++;
    for (int i = phase_len - 1; i > 0; i--) trail[i] = trail[i - 1];
    trail[0] = (Vector2){theta, omega};
}

/*把当前(t, θ)追加到时间序列缓冲, 超出容量则丢弃最老的样本*/
void record_trace() {
    if (trace_len < TRACE_N) trace_len++;
    for (int i = trace_len - 1; i > 0; i--) {
        trace_t[i] = trace_t[i - 1];
        trace_theta[i] = trace_theta[i - 1];
    }
    trace_t[0] = t;
    trace_theta[0] = theta;
}

/*相平面坐标(θ, ω)映射到相图面板像素, θ ∈ [-π, π], ω ∈ [-5, 5]*/
Vector2 phase_pos(double th, double om) {
    Rectangle r = PHASE_RECT;
    return (Vector2){r.x + r.width / (2 * M_PI) * (th + M_PI),
                     r.y + r.height / 10.0 * (5.0 - om)};
}

/*时间序列第i个样本映射到面板像素, 横轴为最近T_WINDOW秒*/
Vector2 trace_pos(int i) {
    Rectangle r = TRACE_RECT;
    return (Vector2){r.x + r.width / T_WINDOW * (trace_t[i] - (t - T_WINDOW)),
                     r.y + r.height / (2 * M_PI) * (M_PI - trace_theta[i])};
}

/*左侧: 摆的实时动画, 摆球处的水平短线指示驱动力矩*/
void draw_pendulum() {
    Vector2 pivot = {WIDTH / 4.0, HEIGHT / 4.0};
    Vector2 bob = {pivot.x + ROD * sin(theta), pivot.y + ROD * cos(theta)};
    Vector2 beam_l = {pivot.x - 30, pivot.y};
    Vector2 beam_r = {pivot.x + 30, pivot.y};
    double drive = F * sin(Omega * t);
    Vector2 tip = {bob.x + 50 * drive, bob.y};

    DrawLineEx(beam_l, beam_r, 3, GRAY);
    DrawLineEx(pivot, bob, LINE_THICK, WHITE);
    DrawCircleV(pivot, 4, LIGHTGRAY);
    DrawCircleV(bob, BOB_RADIUS, RED);
    if (fabs(drive) > 0.02) DrawLineEx(bob, tip, LINE_THICK, SKYBLUE);
}

/*右上: θ-ω相图, 轨迹越新越亮*/
void draw_phase() {
    Rectangle r = PHASE_RECT;
    Color c = BLUE;

    DrawText("theta - omega", r.x, r.y - 24, 16, LIGHTGRAY);
    DrawRectangleLinesEx(r, 1, GRAY);
    DrawLineV(phase_pos(0, -5), phase_pos(0, 5), DARKGRAY);       /*θ = 0*/
    DrawLineV(phase_pos(-M_PI, 0), phase_pos(M_PI, 0), DARKGRAY); /*ω = 0*/

    for (int i = 1; i < phase_len; i++) {
        c.a = 255.0f * (phase_len - i) / phase_len; /*越新越亮*/
        if (fabs(trail[i].x - trail[i - 1].x) > M_PI)
            continue; /*跳过θ卷绕产生的跨面板连线*/
        DrawLineEx(phase_pos(trail[i - 1].x, trail[i - 1].y),
                   phase_pos(trail[i].x, trail[i].y), LINE_THICK, c);
    }
    DrawCircleV(phase_pos(theta, omega), 4, RED);
}

/*右下: 最近T_WINDOW秒的θ-t时间序列*/
void draw_trace() {
    Rectangle r = TRACE_RECT;

    DrawText("theta - t", r.x, r.y - 24, 16, LIGHTGRAY);
    DrawRectangleLinesEx(r, 1, GRAY);
    DrawLineV((Vector2){r.x, r.y + r.height / 2},
              (Vector2){r.x + r.width, r.y + r.height / 2}, DARKGRAY); /*θ = 0*/

    for (int i = 1; i < trace_len; i++) {
        Vector2 p0 = trace_pos(i - 1);
        Vector2 p1 = trace_pos(i);
        if (p1.x < r.x) continue;
        DrawLineEx(p0, p1, 1, GREEN);
    }
    if (trace_len > 0) DrawCircleV(trace_pos(0), 3, RED);
}

int main() {
    srand(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Perturbed Damped Pendulum");
    SetTargetFPS(FPS);
    init();
    FILE *fp = fopen("theta-omega.csv", "w");
    FILE *fp2 = fopen("theta-t.csv", "w");
    fprintf(fp, "theta,omega\n");
    fprintf(fp2, "t,theta\n");

    while (!WindowShouldClose()) {
        /*Space 暂停/继续, R 随机重置, C 清空轨迹*/
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        if (IsKeyPressed(KEY_R)) init();
        if (IsKeyPressed(KEY_C)) {
            phase_len = 0;
            trace_len = 0;
        }

        if (!paused) {
            for (int i = 0; i < SPEED; i++) {
                step(dt);
                steps++;
                if (steps % TRACE_EVERY == 0) record_trace();
                if (steps <= N) { /*与纯数值版一致, 只输出前N步*/
                    fprintf(fp, "%lf,%lf\n", theta, omega);
                    fprintf(fp2, "%lf,%lf\n", t, theta);
                }
            }
            push_trail();
        }

        BeginDrawing();
        ClearBackground(BLACK);
        draw_pendulum();
        draw_phase();
        draw_trace();
        DrawText(TextFormat("t = %.2f s", t), 40, 430, 18, WHITE);
        DrawText(TextFormat("theta = %+.3f rad", theta), 40, 456, 18, WHITE);
        DrawText(TextFormat("omega = %+.3f rad/s", omega), 40, 482, 18, WHITE);
        if (paused) DrawText("PAUSED", 40, 510, 18, YELLOW);
        DrawText("[Space] pause   [R] reset   [C] clear trail", 40, 550, 16, GRAY);
        EndDrawing();
    }

    fclose(fp);
    fclose(fp2);
    CloseWindow();
    printf("finish the simulation, the data is in theta-t.csv and theta-omega.csv\n");
    return 0;
}
