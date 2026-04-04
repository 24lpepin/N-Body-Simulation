#pragma once
#include <iostream>
#include <vector>

#include "geometry.h"

class Object {
    public:
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;

        Object(Vector2D position, Vector2D velocity, Vector2D acceleration, float mass);

        friend std::ostream& operator<<(std::ostream& os, const Object& o);

        void step(const std::vector<Object>& objects, float dt);

        void update_acceleration(const std::vector<Object>& objects);
};