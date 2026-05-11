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



typedef struct DVector2 {
    double x;
    double y;
} DVector2;

static int  flash_timer = 0;
static bool key_was_down = false;

static int count=0;
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
        count++;
        double diff_x = rpos.x - (double)rposf.x;
        double diff_y = rpos.y - (double)rposf.y;
        double diff;
        
        if (count==300){
            diff = sqrt(diff_x * diff_x + diff_y * diff_y);
            count=0;
        }

        DrawCircleLinesV(DVector2ToVector2(center), R, circle_color);
        DrawCircleLinesV(DVector2ToVector2(center), R + 1, circle_color);
        DrawLight();
        DrawText(TextFormat("double: blue  float: red  diff: %.9g px", diff), 12, 12, 18, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
