#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <time.h>

#define WIDTH  800
#define HEIGHT 520
#define R      200
#define V      300
#define SIZE   1
#define FPS    600
#define TRACKER_LINES (FPS * 3)
#define FLASH_FRAMES 20
#define ANGLE_BUF_SIZE 512

#define GRAPH_X  610
#define GRAPH_Y1  40
#define GRAPH_Y2 210
#define GRAPH_W  180
#define GRAPH_H  155



typedef struct DVector2 {
    double x;
    double y;
} DVector2;

static int  flash_timer = 0;
static bool key_was_down = false;

static int   collision_count = 0;
static float angle_history[ANGLE_BUF_SIZE];    // |θd - θf| per collision
static float drift_d_history[ANGLE_BUF_SIZE];  // |θd - θ₀d|
static float drift_f_history[ANGLE_BUF_SIZE];  // |θf - θ₀f|
static int   ah_head  = 0;
static int   ah_count = 0;
static bool  initial_angles_set = false;
static double initial_angle_d = 0.0;
static double initial_angle_f = 0.0;

static DVector2 tracker[TRACKER_LINES];
static Vector2 trackerf[TRACKER_LINES];
static int    tr_head  = 0;
static int    tr_count = 0;

static double v_x, v_y;
static float v_xf, v_yf;
static DVector2 rpos, center;
static Vector2 pos;
static Vector2 rposf, posf, centerf;

static Vector2 DVector2ToVector2(DVector2 p) {
    return (Vector2){(float)p.x, (float)p.y};
}

static void tr_push_both(DVector2 p, Vector2 pf) {
    tracker[tr_head]  = p;
    trackerf[tr_head] = pf;
    tr_head = (tr_head + 1) % TRACKER_LINES;
    if (tr_count < TRACKER_LINES) tr_count++;
}

static DVector2 tr_get(int index) {
    int oldest = (tr_head - tr_count + TRACKER_LINES) % TRACKER_LINES;
    return tracker[(oldest + index) % TRACKER_LINES];
}

static Vector2 tr_get4(int index) {
    int oldest = (tr_head - tr_count + TRACKER_LINES) % TRACKER_LINES;
    return trackerf[(oldest + index) % TRACKER_LINES];
}

static void DrawLight(void) {
    for (int i = 1; i < tr_count; i++) {
        Color color = BLUE;
        color.a = (unsigned char)(255.0f * (i + 1) / tr_count);
        DrawLineEx(DVector2ToVector2(tr_get(i - 1)), DVector2ToVector2(tr_get(i)), 2.0f, color);
    }
    for (int i = 1; i < tr_count; i++) {
        Color color = RED;
        color.a = (unsigned char)(255.0f * (i + 1) / tr_count);
        DrawLineEx(tr_get4(i - 1), tr_get4(i), 2.0f, color);
    }
}

// helper: draw one mini graph
static void DrawMiniGraph(float y0, const char *title,
                          float *buf0, Color c0, const char *l0,
                          float *buf1, Color c1, const char *l1) {
    DrawRectangle(GRAPH_X - 2, y0 - 2, GRAPH_W + 4, GRAPH_H + 4,
                  (Color){0, 0, 0, 180});
    DrawRectangleLines(GRAPH_X - 2, y0 - 2, GRAPH_W + 4, GRAPH_H + 4, DARKGRAY);

    DrawText(title, GRAPH_X + 4, y0 + 2, 10, RAYWHITE);

    if (ah_count < 2) return;

    float px = GRAPH_X + 4, py = y0 + 16;
    float pw = GRAPH_W - 8, ph = GRAPH_H - 38;

    float ymax = 0.0001f;
    for (int i = 0; i < ah_count; i++) {
        if (buf0) { float v = buf0[i]; if (v > ymax) ymax = v; }
        if (buf1) { float v = buf1[i]; if (v > ymax) ymax = v; }
    }
    ymax *= 1.1f;

    for (int i = 0; i <= 3; i++) {
        float y = py + ph * i / 3;
        DrawLine(px, y, px + pw, y, (Color){50, 50, 50, 120});
    }
    DrawText(TextFormat("%.4f", ymax), px, py - 4, 8, RAYWHITE);
    DrawText("0", px, py + ph - 8, 8, RAYWHITE);

    for (int i = 1; i < ah_count; i++) {
        if (buf0) {
            float v0 = buf0[(ah_head - ah_count + i - 1 + ANGLE_BUF_SIZE) % ANGLE_BUF_SIZE];
            float v1 = buf0[(ah_head - ah_count + i + ANGLE_BUF_SIZE) % ANGLE_BUF_SIZE];
            float x0 = px + pw * (i - 1) / (ANGLE_BUF_SIZE - 1);
            float x1 = px + pw * i / (ANGLE_BUF_SIZE - 1);
            DrawLineEx((Vector2){x0, py + ph - ph * v0 / ymax},
                       (Vector2){x1, py + ph - ph * v1 / ymax}, 1.5f, c0);
        }
        if (buf1) {
            float v0 = buf1[(ah_head - ah_count + i - 1 + ANGLE_BUF_SIZE) % ANGLE_BUF_SIZE];
            float v1 = buf1[(ah_head - ah_count + i + ANGLE_BUF_SIZE) % ANGLE_BUF_SIZE];
            float x0 = px + pw * (i - 1) / (ANGLE_BUF_SIZE - 1);
            float x1 = px + pw * i / (ANGLE_BUF_SIZE - 1);
            DrawLineEx((Vector2){x0, py + ph - ph * v0 / ymax},
                       (Vector2){x1, py + ph - ph * v1 / ymax}, 1.5f, c1);
        }
    }

    // legend
    int lx = px;
    if (buf0) { DrawRectangle(lx, py + ph + 4, 6, 6, c0); lx += 10;
                DrawText(l0, lx, py + ph + 2, 8, RAYWHITE); lx += 40; }
    if (buf1) { DrawRectangle(lx, py + ph + 4, 6, 6, c1); lx += 10;
                DrawText(l1, lx, py + ph + 2, 8, RAYWHITE); }
}

static void DrawAngleGraph(void) {
    DrawMiniGraph(GRAPH_Y1, "|error|  |theta_d - theta_f|", angle_history, GREEN, "err", NULL, BLANK, NULL);
    DrawMiniGraph(GRAPH_Y2, "drift  |theta - theta_0|",
                  drift_d_history, BLUE,  "double",
                  drift_f_history, RED,   "float");
    DrawText(TextFormat("collisions: %d", collision_count),
             GRAPH_X + 4, GRAPH_Y2 + GRAPH_H + 4, 10, RAYWHITE);
}

static void init(void) {
    SetRandomSeed((unsigned int)time(NULL));

    double phi1  = GetRandomValue(0, 360) * DEG2RAD;
    double phi2  = GetRandomValue(0, 360) * DEG2RAD;
    double r0    = GetRandomValue(0, R);
    float phi1f  = (float)phi1;
    float phi2f  = (float)phi2;
    float r0f    = (float)r0;

    v_x  = V * cos(phi1);
    v_y  = V * sin(phi1);
    v_xf  = V * cosf(phi1f);
    v_yf  = V * sinf(phi1f);
    rpos = (DVector2){r0 * cos(phi2), r0 * sin(phi2)};
    pos  = DVector2ToVector2((DVector2){rpos.x + center.x, rpos.y + center.y});
    rposf = (Vector2){r0f * cosf(phi2f), r0f * sinf(phi2f)};
    posf  = (Vector2){rposf.x + centerf.x, rposf.y + centerf.y};

    tr_head  = 0;
    tr_count = 0;

    ah_head  = 0;
    ah_count = 0;
    collision_count = 0;
    initial_angles_set = false;
}

static void step(double dt) {
    rpos.x += v_x * dt;
    rpos.y += v_y * dt;

    double r2 = rpos.x * rpos.x + rpos.y * rpos.y;
    if (r2 >= R * R) {
        double len = sqrt(r2);
        double nx  = rpos.x / len;
        double ny  = rpos.y / len;
        double dot = v_x * nx + v_y * ny;

        double angle_d = acos(fabs(dot) / V) * RAD2DEG;
        double dot_f   = (double)v_xf * nx + (double)v_yf * ny;
        double angle_f = acos(fabs(dot_f) / V) * RAD2DEG;
        float  err     = (float)fabs(angle_d - angle_f);

        if (!initial_angles_set) {
            initial_angle_d = angle_d;
            initial_angle_f = angle_f;
            initial_angles_set = true;
        }
        float drift_d = (float)fabs(angle_d - initial_angle_d);
        float drift_f = (float)fabs(angle_f - initial_angle_f);

        angle_history[ah_head]   = err;
        drift_d_history[ah_head] = drift_d;
        drift_f_history[ah_head] = drift_f;
        ah_head = (ah_head + 1) % ANGLE_BUF_SIZE;
        if (ah_count < ANGLE_BUF_SIZE) ah_count++;
        collision_count++;

        v_x -= 2 * dot * nx;
        v_y -= 2 * dot * ny;

        rpos.x = nx * (R - SIZE);
        rpos.y = ny * (R - SIZE);
    }

    pos  = (Vector2){rpos.x + center.x, rpos.y + center.y};

    float dtf = (float)dt;
    rposf.x += v_xf * dtf;
    rposf.y += v_yf * dtf;

    float r2f = rposf.x * rposf.x + rposf.y * rposf.y;
    if (r2f >= R * R) {
        float len = sqrtf(r2f);
        float nx  = rposf.x / len;
        float ny  = rposf.y / len;
        float dot = v_xf * nx + v_yf * ny;

        v_xf -= 2 * dot * nx;
        v_yf -= 2 * dot * ny;

        rposf.x = nx * (R - SIZE);
        rposf.y = ny * (R - SIZE);
    }

    posf = (Vector2){rposf.x + centerf.x, rposf.y + centerf.y};
    tr_push_both((DVector2){rpos.x + center.x, rpos.y + center.y}, posf);
}

int main(void) {
    center = (DVector2){WIDTH / 2.0, HEIGHT / 2.0};
    centerf = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};

    InitWindow(WIDTH, HEIGHT, "Reflection of light in a sphere");
    SetTargetFPS(FPS);
    init();

    while (!WindowShouldClose()) {
        step(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);

        bool key_down = IsKeyDown(KEY_SPACE);
        if (key_down && !key_was_down) {
            printf("RESET triggered!\n");
            init();
            flash_timer = FLASH_FRAMES;
        }
        key_was_down = key_down;

        Color circle_color = (flash_timer > 0) ? RED : WHITE;
        if (flash_timer > 0) flash_timer--;

        DrawCircleLinesV(DVector2ToVector2(center), R, circle_color);
        DrawCircleLinesV(DVector2ToVector2(center), R + 1, circle_color);
        DrawLight();
        DrawAngleGraph();

        static int  frame_cnt = 0;
        static float pos_diff = 0.0f;
        frame_cnt++;
        if (frame_cnt >= FPS) {
            double dx = rpos.x - (double)rposf.x;
            double dy = rpos.y - (double)rposf.y;
            pos_diff = (float)sqrt(dx * dx + dy * dy);
            frame_cnt = 0;
        }
        DrawText(TextFormat("double: blue  float: red"), 12, 12, 18, RAYWHITE);
        DrawText(TextFormat("pos diff: %.6f px", pos_diff), 12, 34, 18, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*
在模拟中我们可以看到
误差主要产生于反射角的计算，而不是欧拉距离
主要原因应该是误差的阶不同
同时由于角度是依赖商实现，更易反应微笑误差
*/