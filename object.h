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

        /**
         * Computes the net force on the object based on the positions of the given list of objects.
         */
        void compute_force(const std::vector<Object>& objects);

    private:
        std::optional<Vector2D> force;

        /**
         * Updates the object's acceleration based on the calculated net force.
         */
        void update_acceleration(const Vector2D& force);
};