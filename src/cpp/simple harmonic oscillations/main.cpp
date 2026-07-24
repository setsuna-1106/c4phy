#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

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
    double get_x()
    {
        return x;
    }

    double get_v()
    {
        return v;
    }

};

int main(){
    oscillator o1(10,0);
    std::ofstream fout("x-v.csv");
    for(int i=0;i<1000;i++){
        o1.fprint_xv(fout);
        o1.step();
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window=
        SDL_CreateWindow(
            "Phase Space",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800,
            600,
            SDL_WINDOW_SHOWN
        );

    SDL_Renderer* renderer=
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );

    bool running=true;

    SDL_Event event;


    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT)
                running=false;
        }

        //背景
        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer);

        //坐标轴
        SDL_SetRenderDrawColor(
            renderer,
            100,
            100,
            100,
            255
        );
        SDL_RenderDrawLine(
            renderer,
            0,
            300,
            800,
            300
        );
        SDL_RenderDrawLine(
            renderer,
            400,
            0,
            400,
            600
        );

        //画相点
        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255
        );
        int px=
            400+o1.get_x()*30;
        int py=
            300-o1.get_v()*3;

        SDL_RenderDrawPoint(
            renderer,
            px,
            py
        );
        SDL_RenderPresent(renderer);
        o1.step();
        SDL_Delay(20);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}