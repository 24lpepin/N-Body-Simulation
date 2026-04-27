#include <iostream>
#include <vector>
#include <queue>
#include "geometry.h"
#include "object.h"
#include "const.h"

Object::Object(Vector2D position, Vector2D velocity, Vector2D acceleration, double mass): 
    position(position), velocity(velocity), acceleration(acceleration), mass(mass) 
{ 
    path = std::deque<sf::Vector2f>();
}

std::ostream& operator<<(std::ostream& os, const Object& o) {
    return os << "position: " << o.position << std::endl 
              << "velocity: " << o.velocity << std::endl 
              << "acceleration: " << o.acceleration;
}

bool operator==(const Object& o1, const Object& o2) {
    return (o1.position == o2.position) && (o1.velocity == o2.velocity) && 
            (o1.acceleration == o2.acceleration) && (o1.mass == o2.mass);
}

void Object::update_path() {
    path.push_back(position);
    if (path.size() > MAX_PATH_LEN) {
        path.pop_front();
    }
}

Vector2D Object::compute_force(const std::vector<Object>& objects) {
    Vector2D force = Vector2D(0, 0);
    for (const Object& object : objects) {
        if (object == *this) continue;
        Vector2D r = object.position - position;
        double r2 = r.x * r.x + r.y * r.y + EPSILON * EPSILON;
        double r_mag = sqrt(r2);
        double f = G * object.mass * mass / r2;
        force = force + (f / r_mag) * r;
    }
    return force;
}

double Object::get_kinetic_energy() const {
    return 0.5 * mass * (velocity.x * velocity.x + velocity.y * velocity.y);
}

/**
 * Updates the acceleration by computing the force due to each object in the simulation.
 */
void Object::update_acceleration(const Vector2D& force) {
    acceleration = force * (1.0f / mass);
}