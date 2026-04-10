    #pragma once
    #include <iostream>

class Vector2D {
    public:
        double x;
        double y;

        Vector2D(double x, double y);

        double magnitude();
        
        Vector2D normalize();

        friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

        friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);

        friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);

        friend Vector2D operator-(const Vector2D& v1);

        friend double operator*(const Vector2D& v1, const Vector2D& v2);
        friend Vector2D operator*(double c, const Vector2D& v);
        friend Vector2D operator*(const Vector2D& v, double c);
};
