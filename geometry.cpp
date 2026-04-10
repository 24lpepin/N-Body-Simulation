#include <iostream>
#include <cmath>

#include "geometry.h"

Vector2D::Vector2D(double x, double y): x(x), y(y) { }

double Vector2D::magnitude() {
    return std::sqrt(this->x * this->x + this->y * this->y);
}

Vector2D Vector2D::normalize() {
    double magnitude = this->magnitude();
    return Vector2D(x / magnitude, y / magnitude);
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

Vector2D operator-(const Vector2D& v) {
    return Vector2D(-v.x, -v.y);
}

double operator*(const Vector2D& v1, const Vector2D& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector2D operator*(double c, const Vector2D& v) {
    return Vector2D(c * v.x, c * v.y);
}

Vector2D operator*(const Vector2D& v, double c) {
    return Vector2D(c * v.x, c * v.y);
}