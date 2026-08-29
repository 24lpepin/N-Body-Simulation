#include "direct_force_calculator.h"
#include "../object.h"
#include "../geometry.h"
#include "../const.h"

void DirectForceCalculator::update_accelerations(std::vector<Object>& objects) {
    for (auto& obj1 : objects) {
        Vector2D force = Vector2D(0, 0);

        for (const Object& obj2 : objects) {
            if (obj1 == obj2) continue;
            Vector2D r = obj2.position - obj1.position;
            double r2 = r.x * r.x + r.y * r.y + EPSILON * EPSILON;
            double r_mag = sqrt(r2);
            double f = G * obj1.mass * obj2.mass / r2;
            force = force + (f / r_mag) * r;
        }

        obj1.acceleration = force / obj1.mass;
    }
}
