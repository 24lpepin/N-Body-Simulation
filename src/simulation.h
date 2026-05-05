#include <iostream>
#include <vector>
#include "geometry.h"
#include "object.h"

class Simulation {
public:
    std::vector<Object> objects;

    Simulation(std::vector<Object> objects = std::vector<Object>{});

    Object add_random_object();
    std::vector<Object> add_random_objects(int n);
    std::vector<Object> add_objects(const std::vector<Object>& objects);

    void step(double dt);
    void update_paths();

    void clear();
};