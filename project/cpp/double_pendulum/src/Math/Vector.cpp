#include "Math/Vector.h"
#include <cmath>

vector2D vector2D::operator+(const vector2D& other) const {
    return vector2D(x+other.x,y+other.y);
}

vector2D vector2D::operator*(const double& other) const {
    return vector2D(x*other,y*other);
}

double vector2D::length(){
    return std::sqrt(x*x+y*y);
}