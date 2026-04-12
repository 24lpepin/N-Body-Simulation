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

bool operator==(const Object& o1, const Object& o2) {
    return (o1.position == o2.position) && (o1.velocity == o2.velocity) && 
            (o1.acceleration == o2.acceleration) && (o1.mass == o2.mass);
}

/**
 * Uses standard implementation of velocity Verlet algorithm.
 */
void Object::step(const std::vector<Object>& objects, float dt) {
    Vector2D force = compute_force(objects);
    velocity = velocity + 0.5 * acceleration * dt;
    position = position + velocity * dt;
    update_acceleration(force);
    velocity = velocity + 0.5 * acceleration * dt;
}

Vector2D Object::compute_force(const std::vector<Object>& objects) {
    Vector2D force = Vector2D(0, 0);
    for (const Object& object : objects) {
        if (object == *this) continue;
        Vector2D r = object.position - position;
        float r2 = r.x * r.x + r.y * r.y + EPSILON * EPSILON;
        float r_mag = sqrt(r2);
        float f = G * object.mass * mass / r2;
        force = force + (f / r_mag) * r;
    }
    return force;
}

/**
 * Updates the acceleration by computing the force due to each object in the simulation.
 */
void Object::update_acceleration(const Vector2D& force) {
    acceleration = force * (1.0f / mass);
}