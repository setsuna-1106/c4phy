#include <stdio.h>

#define mu 2
#define x0 0.3

int main(){
    double x;
    FILE *fp=fopen("x.csv","w");
    x=x0;

    for(int i=0;i<1000;i++){
        x=mu*x*(1-x);
        fprintf(fp,"%d,%lf\n",i,x);
    }

    return 0;
}