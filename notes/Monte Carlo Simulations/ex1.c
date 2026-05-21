#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000

int main(){
    double sum=0;
    srand(time(NULL));
    for(int i =0;i<N;i++){
        sum+= (double)rand()/(double)N/(double)RAND_MAX;
    }
    printf("%lf",sum);
    return 0;
}
