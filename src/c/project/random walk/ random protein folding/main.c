#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <raylib.h>

#define WIDTH 101
#define HEIGHT 101
#define LINE 50
#define NUMBER 100


typedef struct{
    int x;
    int y;
}Position;


Position pos[NUMBER][LINE];
int energy[NUMBER];
int map[WIDTH][HEIGHT];

void init(){
    for(int i=0;i<WIDTH;i++){
        for(int j=0;i<HEIGHT;j++){
            map[i][j]=0;
        }
    }
    for(int i=0;i<NUMBER;i++){
        energy[i]=0;
    }
}

int generate(){
    srand(time(NULL));
    int p = GetRandomValue(1,100);
    if(p<70){
        return 1;
        // 生成H端
    }else{
        return 2;
        // 生成P端
    }
}
void step(){
    for(int i=0;i<NUMBER;i++){

    }
}

int main (){

}