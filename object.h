#pragma once
#include <iostream>
#include <vector>
#include "geometry.h"

/**
 * The Object class stores relevant information about a simulation object and provides methods to 
 * update said information.
 */
class Object {
    public:
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;

        Object(Vector2D position, Vector2D velocity, Vector2D acceleration, float mass);

        friend std::ostream& operator<<(std::ostream& os, const Object& o);
        friend bool operator==(const Object& o1, const Object& o2);

        /**
         * Takes in a list of simulation objects and a timestep dt and updates the object's 
         * position, velocity, and acceleration.
         */
        void step(const std::vector<Object>& objects, float dt);

    // private:
        Vector2D compute_force(const std::vector<Object>& objects);

        /**
         * Updates the object's acceleration based on the position of the given list of objects.
         */
        void update_acceleration(const Vector2D& force);
};