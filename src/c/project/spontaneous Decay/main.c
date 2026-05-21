#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000000
#define T 200
#define P 10

int p[N];
long number,count;

void init(){
    srand(time(NULL));
    for(int i=0;i<N;i++){
        p[i]=1;
    }
    number = 0;
    count = 0;
}

void step(){
    number=0;

    for(int i=0;i<N;i++){
        if(p[i]){
            number++;
        }
    }

    for(int i=0;i<N;i++){
        if(p[i]){
            if(rand()%100<P ){
                p[i]=0;
            }
        }
    }

    count++;
}

int main (int argc, char **argv){
    init();
    FILE *fp=fopen("number_of_particles.csv","w");
    for(int i=0;i<T;i++){
        step();
        fprintf(fp,"%ld,%ld\n",count,number);
    }

    fclose(fp);
    printf("finish the simulation,the data is in number_of_particles.csv");
}
