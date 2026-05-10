#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

#define WIDTH  800
#define HEIGHT 520
#define R      200
#define V      300
#define SIZE   1
#define FPS    600
#define TRACKER_LINES (FPS * 3)
#define FLASH_FRAMES 20

static int  flash_timer = 0;
static bool key_was_down = false;

static Vector2 tracker[TRACKER_LINES];
static int    tr_head  = 0;
static int    tr_count = 0;

static double v_x, v_y;
static Vector2 rpos, pos, center;

static void tr_push(Vector2 p) {
    tracker[tr_head] = p;
    tr_head = (tr_head + 1) % TRACKER_LINES;
    if (tr_count < TRACKER_LINES) tr_count++;
}

static Vector2 tr_get(int index) {
    int oldest = (tr_head - tr_count + TRACKER_LINES) % TRACKER_LINES;
    return tracker[(oldest + index) % TRACKER_LINES];
}

static void DrawLight(void) {
    for (int i = 0; i < tr_count; i++) {
        Color color = BLUE;
        color.a = (unsigned char)(255.0f * (i + 1) / tr_count);
        DrawRectangleV(tr_get(i), (Vector2){SIZE, SIZE}, color);
    }
}

static void init(void) {
    SetRandomSeed((unsigned int)time(NULL));

    double phi1  = GetRandomValue(0, 360) * DEG2RAD;
    double phi2  = GetRandomValue(0, 360) * DEG2RAD;
    double r0    = GetRandomValue(0, R);

    v_x  = V * cos(phi1);
    v_y  = V * sin(phi1);
    rpos = (Vector2){r0 * cos(phi2), r0 * sin(phi2)};
    pos  = (Vector2){rpos.x + center.x, rpos.y + center.y};

    tr_head  = 0;
    tr_count = 0;
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

        v_x -= 2 * dot * nx;
        v_y -= 2 * dot * ny;

        rpos.x = nx * (R - SIZE);
        rpos.y = ny * (R - SIZE);
    }

    pos = (Vector2){rpos.x + center.x, rpos.y + center.y};
    tr_push(pos);
}

int main(void) {
    center = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};

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

        DrawCircleLinesV(center, R, circle_color);
        DrawCircleLinesV(center, R + 1, circle_color);
        DrawLight();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
