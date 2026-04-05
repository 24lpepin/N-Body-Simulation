#include <iostream>
#include <vector>
#include "geometry.h"
#include "object.h"
#include "const.h"

Object::Object(Vector2D position, Vector2D velocity, Vector2D acceleration, float mass): position(position), 
    velocity(velocity), acceleration(acceleration), mass(mass) {}

std::ostream& operator<<(std::ostream& os, const Object& o) {
    return os << "position: " << o.position << std::endl 
              << "velocity: " << o.velocity << std::endl 
              << "acceleration: " << o.acceleration;
}

/**
 * Uses standard implementation of velocity Verlet algorithm.
 */
void Object::step(const std::vector<Object>& objects, float dt) {
    velocity = velocity + 0.5 * acceleration * dt;
    position = position + velocity * dt;
    update_acceleration(objects);
    velocity = velocity + 0.5 * acceleration * dt;
}

/**
 * Updates the acceleration by computing the force due to each object in the simulation.
 */
void Object::update_acceleration(const std::vector<Object>& objects) {
    acceleration = Vector2D(0,0);
    for (const Object& object : objects) {
        if (&object == this) continue; // Skip the calling object

        Vector2D r = object.position - position;
        float r2 = r.magnitude() * r.magnitude();
        float a = G * object.mass / r2;
        acceleration = acceleration + a * r.normalize();
    }
}