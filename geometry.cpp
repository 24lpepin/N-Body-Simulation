#include <iostream>
#include <cmath>

#include "geometry.h"

Vector2D::Vector2D(double x, double y): x(x), y(y) { }

double Vector2D::magnitude() {
    return std::sqrt(this->x * this->x + this->y * this->y);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

Vector2D operator-(const Vector2D& v1) {
    return Vector2D(-v1.x, -v1.y);
}

double operator*(const Vector2D& v1, const Vector2D& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}