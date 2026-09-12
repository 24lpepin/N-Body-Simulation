#include "object_loader.h"
#include "object.h"

#include <fstream>
#include <vector>
#include <random>

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

std::vector<Object> ObjectLoader::create_objects(int n, unsigned seed) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> pos_dist(-1e5, 1e5);
    std::uniform_real_distribution<double> vel_dist(-10.0, 10.0);
    std::uniform_real_distribution<double> mass_dist(1e20, 1e25);

    std::vector<Object> objects;
    for (int i = 0; i < n; i++) {
        Object obj({pos_dist(rng), pos_dist(rng)}, {vel_dist(rng), vel_dist(rng)}, mass_dist(rng));
        objects.push_back(obj);
    }
    return objects;
}

std::vector<Object> ObjectLoader::load_objects(int id) {
    std::ifstream f("./assets/initial_configurations.json");
    json configurations = json::parse(f);

    std::vector<Object> objects;

    for (auto& config : configurations) {
        if (config["id"] == id) { // TODO protect against duplicate ids
            for (auto& body : config["bodies"]) {
                Vector2D position(body["position"][0], body["position"][1]);
                Vector2D velocity(body["velocity"][0], body["velocity"][1]);
                objects.push_back(Object(position, velocity, body["mass"], 0, body["color"]));
                // objects.push_back(Object(position, velocity, body["mass"], 1));
            }
            
        }
    }

    return objects;
}