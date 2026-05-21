#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000

int main (){
    srand(time(NULL));
    double sum;
    for (int i=0;i<N;i++){
        sum += (double)rand()*rand()/N/RAND_MAX/RAND_MAX;
    }
    printf("%lf",sum);
    return 0;
}
