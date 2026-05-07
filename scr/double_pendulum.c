#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define WIDTH 900
#define HEIGHT 600
#define MASS_RADIUS 10
#define LINE_THICK 2

/*初始化物理量*/
#define L1 300
#define L2 200
#define M1 1
#define M2 1
#define g 980
/*长度使用cm作为单位*/

Vector2 start=(Vector2){WIDTH/2,0};

float l1,l2,m1,m2,phi1,phi2,phi1_d,phi2_d,phi1_dd,phi2_dd;

/*
Vector2 get_end(float l,Vector2 start,float phi)
{
    Vector2 end=(Vector2){start.x+l*sinf(phi),start.y+l*cosf(phi)};
    return end;
}
*/
void draw_pendulum(float l,Vector2 start,float phi)
{
    Vector2 end = {start.x+l*sinf(phi),start.y+l*cosf(phi)};
    DrawLineEx(start,end,LINE_THICK,WHITE);
    DrawCircleV(end,MASS_RADIUS,RED);
}


void draw_double_pendulum(float l1,float l2,Vector2 start,float phi1,float phi2){
    Vector2 end1={start.x+l1*sinf(phi1),start.y+l1*cosf(phi1)};
    draw_pendulum(l1,start,phi1);
    draw_pendulum(l2,end1,phi2);
}

/*初始化求解器*/
void init_solver()
{
    srand(time(NULL));
    
    l1=L1;
    l2=L2;
    m1=M1;
    m2=M2;
    phi1_d=0;
    phi2_d=0;
    phi1_dd=0;
    phi2_dd=0;

    phi1=GetRandomValue(-90,90)*DEG2RAD;
    phi2=GetRandomValue(-90,90)*DEG2RAD;
}

void step(float dt)
{
    phi1_dd = (-g*(2*m1+m2)*sinf(phi1) - m2*g*sinf(phi1-2*phi2) - 2*sinf(phi1-phi2)*m2*(phi2_d*phi2_d*l2 + phi1_d*phi1_d*l1*cosf(phi1-phi2)))/ (l1 * (2*m1+m2-m2*cosf(2*phi1-2* phi2)));
    phi2_dd = 2*sinf(phi1-phi2)*(phi1_d*phi1_d*l1*(m1+m2) + g*(m1+m2)*cosf(phi1) + phi2_d*phi2_d*l1*m2*cosf(phi1-phi2)) / (l2 * (2*m1+m2-m2*cosf(2*phi1 - 2*phi2)));

    phi1_d += phi1_dd*dt;
    phi2_d += phi2_dd*dt;

    phi1 += phi1_d*dt;
    phi2 += phi2_d*dt;
}
/*步进函数数求解器*/

int main(int argc,char *argv[]){
    InitWindow(WIDTH,HEIGHT,"Double Pendulum");
    SetTargetFPS(100);
    init_solver();
    while(!WindowShouldClose()){
        step(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);
        draw_double_pendulum(l1,l2,start,phi1,phi2);
        
        EndDrawing();
    }
    return 0;    
}