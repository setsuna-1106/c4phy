#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>


#define WIDTH 800
#define HEIGHT 520
#define FPS 100

#define number 100
#define show_number 4
#define R 1

Vector2 pos1[number],pos2[number];
Color color[show_number]={RED,WHITE,BLUE,GREEN};
double rms1,rms2;
int count=0;

void init(){
    for(int i=0;i<number;i++){
        pos1[i].x=WIDTH/2;
        pos1[i].y=HEIGHT/2;
        pos2[i].x=WIDTH/2;
        pos2[i].y=HEIGHT/2;
        rms1 = 0;
        rms2 = 0;
        SetRandomSeed(time(NULL));
    }
}

void step(){
    rms1 =rms2;
    rms2 =0;
    for(int i=0;i<number;i++){
        float phi=GetRandomValue(0,360)*DEG2RAD;
        pos1[i].x =pos2[i].x;
        pos1[i].y =pos2[i].y;
        
        pos2[i].x +=R*cosf(phi);
        pos2[i].y +=R*sinf(phi);
        rms2+=(pos2[i].x-WIDTH/2)*(pos2[i].x-WIDTH/2)+(pos2[i].y-HEIGHT/2)*(pos2[i].y-HEIGHT/2);
    }
    rms2=sqrt(rms2/number);
    count++;

}

int main(){
    InitWindow(WIDTH,HEIGHT,"random walk");
    SetTargetFPS(FPS);    
    init();
    FILE *fp=fopen("rw.csv","w");
    if (fp == NULL) {
        perror("fopen failed");
        return 1;
    }

    while(!WindowShouldClose()){
        step();
        fprintf(fp,"%d,%lf\n",count,rms2);
        BeginDrawing();
        for(int i=0;i<show_number;i++){
            DrawLineEx(pos1[i],pos2[i],1,color[i]);
        }
        fprintf(fp,"%d,%lf\n",count,rms2);
        // DrawLineEx((Vector2){count,HEIGHT-rms1-20},(Vector2){count,HEIGHT-rms2-20},1,WHITE);

        EndDrawing();
    }
    fclose(fp);
    // char command[256];
    // snprintf(command, sizeof(command), "random walk.py");
    // int ret = system(command);

    // if (ret != 0) {
    //     printf("Python program failed.\n");
    // }

    return 0;
}