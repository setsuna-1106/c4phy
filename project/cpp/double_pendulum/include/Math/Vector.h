#pragma once

class vector2D{
    public:
    double x,y;

    vector2D():x(0),y(0){}
    vector2D(double x_,double y_):x(x_),y(y_){}

    double length();

    vector2D operator+(const vector2D& other)const;
    vector2D operator*(const double& other)const;
};