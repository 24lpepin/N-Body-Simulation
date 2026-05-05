#pragma once
#include <iostream>
#include <vector>
#include <queue>
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
        double mass;
        std::deque<sf::Vector2f> path;
        int id;

        Object(Vector2D position, Vector2D velocity, double mass, int id = 0);

        friend std::ostream& operator<<(std::ostream& os, const Object& o);
        friend bool operator==(const Object& o1, const Object& o2);

        void update_path();

        /**
         * Computes the net force on the object based on the positions of the given list of objects.
         */
        Vector2D compute_force(const std::vector<Object>& objects);

        double get_kinetic_energy() const;

    private:
        /**
         * Updates the object's acceleration based on the calculated net force.
         */
        void update_acceleration(const Vector2D& force);
};