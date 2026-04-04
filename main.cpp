#include <iostream>
#include <cmath>

#include "geometry.h"

int main() {
    Vector2D v1(2,3);
    Vector2D v2(-1,1.5);
    std::cout << v1 + v2 << std::endl;
    std::cout << -v2 << std::endl;
    std::cout << v1.magnitude() << std::endl;
    return 0;
}
