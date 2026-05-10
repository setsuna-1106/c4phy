#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 520
#define R 200
#define V 300
#define SIZE 1
#define TRACKER_LINES FPS*3
#define FPS 600

double v_x,v_y,x,y,phi1,phi2;
Vector2 tracker[TRACKER_LINES],rpos,pos,center={WIDTH/2,HEIGHT/2};
int tracker_t_l=0;

double random_double(double min, double max)
{
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

void DrawLight() {
    if (tracker_t_l < TRACKER_LINES) tracker_t_l++;

    for (int i = tracker_t_l - 1; i > 0; i--) {
        tracker[i] = tracker[i - 1];
    }

    tracker[0] = pos;

    for (int i = 0; i < tracker_t_l; i++) {
        Color color = BLUE;
        color.a = (unsigned char)(255.0f * (tracker_t_l - i) / tracker_t_l);
        DrawRectangleV(tracker[i], (Vector2){SIZE, SIZE}, color);
    }
}

void init(){
    srand(time(NULL));
    phi1=GetRandomValue(0,360)*DEG2RAD;
    phi2=GetRandomValue(0,360)*DEG2RAD;
    double r=GetRandomValue(0,R);
    v_x=V*cos(phi1);
    v_y=V*sin(phi1);
    x=r*cos(phi2);
    y=r*sin(phi2);
    rpos=(Vector2){x,y};
    pos=(Vector2){rpos.x+center.x,rpos.y+center.y};
}

void step(double dt){
    rpos.x += v_x * dt;
    rpos.y += v_y * dt;

    if((rpos.x * rpos.x + rpos.y * rpos.y) >= R * R){
        double len = sqrt(rpos.x * rpos.x + rpos.y * rpos.y);

        double nx = rpos.x / len;
        double ny = rpos.y / len;

        double dot = v_x * nx + v_y * ny;

        v_x = v_x - 2 * dot * nx;
        v_y = v_y - 2 * dot * ny;

        rpos.x = nx * (R - SIZE);
        rpos.y = ny * (R - SIZE);
    }
    pos = (Vector2){rpos.x + center.x, rpos.y + center.y};
}

int main (){
    InitWindow(WIDTH,HEIGHT,"Reflection of light in a sphere");
    SetTargetFPS(FPS);
    init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        step(GetFrameTime());
        DrawCircleLinesV(center, R, WHITE);
        DrawCircleLinesV(center, R+1, WHITE);
        DrawLight();

    EndDrawing();
}

    return 0;
}