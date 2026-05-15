#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define WIDTH 900
#define HEIGHT 600
#define MASS_RADIUS 10
#define LINE_THICK 2
#define FPS 500

/*初始化物理量*/
#define L1 300
#define L2 200
#define M1 1
#define M2 1
#define g 980
/*长度使用cm作为单位*/

#define TRACKER_LINE 500
#define TRACKER_SIZE 3
Vector2 tracker[TRACKER_LINE];
int tracker_now_l;
Vector2 start=(Vector2){WIDTH/2,0};

double l1,l2,m1,m2,phi1,phi2,phi1_d,phi2_d,phi1_dd,phi2_dd;
double E,E_i,delta;
int count,count1;


Vector2 get_end(double l,Vector2 start,double phi)
{
    Vector2 end=(Vector2){start.x+l*sinf(phi),start.y+l*cosf(phi)};
    return end;
}

void draw_pendulum(double l,Vector2 start,double phi)
{
    Vector2 end = get_end(l,start,phi);
    DrawLineEx(start,end,LINE_THICK,WHITE);
    DrawCircleV(end,MASS_RADIUS,RED);
}


void draw_double_pendulum(double l1,double l2,Vector2 start,double phi1,double phi2){
    Vector2 end1=get_end(l1,start,phi1);
    Vector2 end2=get_end(l2,end1,phi2);
    draw_pendulum(l1,start,phi1);
    draw_pendulum(l2,end1,phi2);
    
    count1++;
    if(count1==5){
        if(tracker_now_l<99) tracker_now_l++;
    
        for(int i=tracker_now_l;i>0;i--)
        {
        tracker[i]=tracker[i-1];
        }               
    count1=0;
    }
    Color color=BLUE;
    tracker[0]=end2;
    for(int i=0;i<tracker_now_l;i++)
    {   
        color.a=(unsigned char)(float)0xFF*(float)(tracker_now_l-i)/(float)tracker_now_l;
        DrawRectangleV(tracker[i],(Vector2){TRACKER_SIZE,TRACKER_SIZE},color);
    }
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
    
    E=0.5*(m1+m2)*l1*l1*phi1_d*phi1_d+
    0.5*m2*l2*l2*phi2_d*phi2_d+m2*l1*l2*phi1_d*phi2_d*cos(phi1-phi2)-
    (m1+m2)*g*l1*cos(phi1)-
    m2*g*l2*cos(phi2);
    E_i=E;
    delta=0;
    count=0;
    count=1;

    tracker_now_l=0;
}

void step(double dt)
{
    phi1_dd = (-g*(2*m1+m2)*sin(phi1) - m2*g*sin(phi1-2*phi2) - 2*sin(phi1-phi2)*m2*(phi2_d*phi2_d*l2 + phi1_d*phi1_d*l1*cos(phi1-phi2)))/ (l1 * (2*m1+m2-m2*cos(2*phi1-2* phi2)));
    phi2_dd = 2*sin(phi1-phi2)*(phi1_d*phi1_d*l1*(m1+m2) + g*(m1+m2)*cos(phi1) + phi2_d*phi2_d*l1*m2*cos(phi1-phi2)) / (l2 * (2*m1+m2-m2*cos(2*phi1 - 2*phi2)));

    phi1_d += phi1_dd*dt;
    phi2_d += phi2_dd*dt;

    phi1 += phi1_d*dt;
    phi2 += phi2_d*dt;
    
    E=0.5*(m1+m2)*l1*l1*phi1_d*phi1_d+
    0.5*m2*l2*l2*phi2_d*phi2_d+m2*l1*l2*phi1_d*phi2_d*cos(phi1-phi2)-
    (m1+m2)*g*l1*cos(phi1)-
    m2*g*l2*cos(phi2);
    
    count ++;
    if(count==(int)(FPS*500)){
        delta=(E-E_i)/E_i;
        count=0;
    }    

}   
/*步进函数数求解器*/


int main(int argc,char *argv[]){
    InitWindow(WIDTH,HEIGHT,"Double Pendulum");
    SetTargetFPS(FPS);
    init_solver();
    while(!WindowShouldClose()){
        for(int i=0;i<1000;i++) step(GetFrameTime()/1000);
        BeginDrawing();
        ClearBackground(BLACK);
        draw_double_pendulum(l1,l2,start,phi1,phi2);
        
        DrawText(TextFormat("Relative energy error %.2lf%%",delta*100), 50, 50, 10, WHITE);
        EndDrawing();
    }
    return 0;    
}