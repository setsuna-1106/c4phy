#pragma once
#include "Math/Vector.h"

class particle{
    private:
    double mass;
    public:
    vector2D position;
    vector2D velocity;
    
    particle();
    particle(double mass_,vector2D p,vector2D v);

    
};