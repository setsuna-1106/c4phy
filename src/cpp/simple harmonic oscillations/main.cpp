#include <iostream>
#include <fstream>

constexpr double k=50;
constexpr double m=1;
constexpr double dt=1e-2;

class oscillator{
    private:
    double k=50;
    double m=1;
    double dt=1e-2;
    double x;
    double v;
    int tick=0;

    public:
    oscillator(){
        x=0;
        v=0;
    }

    oscillator(double x_,double v_)
    :x(x_),v(v_){}

    void fprint_xv(std::ofstream& fout){
        fout<<x<<","<<v<<std::endl;
    }

    void step(){
        double a;
        a=-k*x/m;
        v=v+a*dt;
        x=x+v*dt;
        tick++;
    }

};

int main(){
    oscillator o1(10,0);
    std::ofstream fout("x-v.csv");
    for(int i=0;i<1000;i++){
        o1.fprint_xv(fout);
        o1.step();
    }

    return 0;
}