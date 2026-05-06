#include <iostream>
#include <vector>
#include <random>
#include "geometry.h"
#include "object.h"
#include "const.h"
#include "simulation.h"

Simulation::Simulation(std::vector<Object> objects): objects(objects) {}

Object Simulation::add_random_object() {
    return add_random_objects(1)[0];
}

std::vector<Object> Simulation::add_random_objects(int n) {
    // double m = std::pow(10, 15);
    // double m = 1;
    std::vector<Object> new_objects{};

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> uniform_distrib(0, 1);
    // std::normal_distribution<> normal_distrib(400, 125);
    std::normal_distribution<> normal_distrib(0, 1);

    for (int i = 0; i < n; i++) {
        Vector2D x(normal_distrib(gen), normal_distrib(gen));
        Vector2D v((2 * uniform_distrib(gen) - 1), (2 * uniform_distrib(gen) - 1));
        double m = uniform_distrib(gen);
        v = sqrt(G) * v;
        // Object object(x, v, {0,0}, uniform_distrib(gen) * m);
        int id = round(COLORS.size() * uniform_distrib(gen));
        Object object(x, v, m, id);
        objects.push_back(object);
        new_objects.push_back(object);
    }
    return new_objects;
}

std::vector<Object> Simulation::add_objects(const std::vector<Object>& objs) {
    for (auto& obj : objs) {
        objects.push_back(obj);
    }
    return objs;
}

void Simulation::step(double dt) {
    // Compute forces at t
    for (auto& obj : objects) {
        obj.acceleration = obj.compute_force(objects) / obj.mass;
    }

    // Half-step velocity + position
    for (auto& obj : objects) {
        obj.velocity = obj.velocity + 0.5 * obj.acceleration * dt;
        obj.position = obj.position + obj.velocity * dt;
    }

    // Recompute forces at t + dt
    for (auto& obj : objects) {
        obj.acceleration = obj.compute_force(objects) / obj.mass;
    }

    // Finalize velocity
    for (auto& obj : objects) {
        obj.velocity = obj.velocity + 0.5 * obj.acceleration * dt;
    }

    for (int i = objects.size() - 1; i >= 0; i--) {
        if (objects[i].position.magnitude() >= 50) {
            std::cout << "Object " << i << " has been removed" << std::endl;
            objects.erase(objects.begin() + i);
        }
    }
}

void Simulation::update_paths() {
    for (auto& obj : objects) {
        obj.update_path();
    }
}

void Simulation::clear() {
    objects.clear();
}