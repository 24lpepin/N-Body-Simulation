#include <iostream>
#include <vector>
#include "geometry.h"
#include "object.h"
#include "forces/force_calculator.h"

class Simulation {
public:
    std::unique_ptr<ForceCalculator> force_calculator;
    std::vector<Object> objects;

    Simulation(
        std::unique_ptr<ForceCalculator> force_calculator, 
        std::vector<Object> objects = std::vector<Object>{}
    );

    Object add_random_object();
    std::vector<Object> add_random_objects(int n);
    std::vector<Object> add_objects(const std::vector<Object>& objects);

    void step(double dt);
    void update_paths();

    void clear();

    double compute_total_energy();
    double compute_total_angular_momentum();

private:
    double pairwise_potential(const Object& a, const Object& b);
    double compute_total_potential_energy();
};