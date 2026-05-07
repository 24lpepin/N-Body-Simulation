#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <queue>

#include "geometry.h"
#include "object.h"
#include "const.h"
#include "simulation.h"
#include "renderer.h"

#include <./SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

std::vector<Object> create_objects(int n = 1) { // TODO move this to simulation? also add param for name
    std::ifstream f("../assets/initial_configurations.json");
    json configurations = json::parse(f);

    std::vector<Object> objects;

    for (auto& config : configurations) {
        if (config["id"] == n) { // TODO protect against duplicate ids
            for (auto& body : config["bodies"]) {
                Vector2D position(body["position"][0], body["position"][1]);
                Vector2D velocity(body["velocity"][0], body["velocity"][1]);

                if (config["G"] == 1) {
                    velocity = velocity * sqrt(G); // Simulation uses G=39.478. Need to normalize
                }

                objects.push_back(Object(position, velocity, body["mass"], 0, body["color"]));
                // objects.push_back(Object(position, velocity, body["mass"], 1));
            }
            
        }
    }

    return objects;
}

double pairwise_potential(const Object& a, const Object& b) {
    double r = (a.position - b.position).magnitude();
    return -G * a.mass * b.mass / r;
}

double compute_total_potential_energy(const std::vector<Object>& objects) {
    double total = 0.0;

    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            total += pairwise_potential(objects[i], objects[j]);
        }
    }

    return total;
}

double compute_total_energy(const std::vector<Object>& objects) {
    double kinetic = 0.0;

    for (const Object& obj : objects) {
        kinetic += obj.get_kinetic_energy();
    }

    double potential = compute_total_potential_energy(objects);

    return kinetic + potential;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Orbital Simulation");
    window.setPosition({100,100});
    window.setVerticalSyncEnabled(true);

    Renderer renderer(window);

    bool paused = false;
    
    double dt = 0.00002f;
    double time = 0.0f;
    int steps = 0;
    const int draw_buffer = 10;
    const int print_buffer = draw_buffer * 200;
    
    const int n = 2;
    Simulation simulation(create_objects(n));

    sf::Clock clock;

    double initial_energy = compute_total_energy(simulation.objects);

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float fps = 1.0f / elapsed.asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                const sf::Keyboard::Key code = event->getIf<sf::Event::KeyPressed>()->code;
                std::vector<Object> objs;

                switch (code)
                {
                case sf::Keyboard::Key::Escape: // Close window
                    window.close();
                    break;

                case sf::Keyboard::Key::P: // Pause window
                    paused = !paused;
                    break;
                
                case sf::Keyboard::Key::C: // Clear window
                    simulation.clear();
                    renderer.clear();
                    break;

                case sf::Keyboard::Key::R: // Reset configuration
                    simulation.clear();
                    renderer.clear();
                    objs = simulation.add_objects(create_objects(n));
                    renderer.draw(objs);
                    initial_energy = compute_total_energy(simulation.objects); // recompute initial energy
                    break;

                case sf::Keyboard::Key::B: // Adds a new object in a random location
                {
                    Object object = simulation.add_random_object();
                    initial_energy = compute_total_energy(simulation.objects); // recompute initial energy
                    renderer.draw(object);
                    break;
                }

                case sf::Keyboard::Key::Equal:
                    dt *= 1.5;
                    break;

                case sf::Keyboard::Key::Hyphen:
                    dt /= 1.5;
                    break;
                }
            }
        }

        renderer.display(); // redraw window here incase window is paused.

        if (paused) {
            continue;
        }

        if (steps % draw_buffer == 0) { 
            renderer.clear();
            simulation.update_paths();
            renderer.draw(simulation.objects);
        }
        
        for (int i = 0; i < simulation.objects.size(); i++) {
            Object& obj = simulation.objects[i];

            if (steps % print_buffer == 0) {

                double drift = 100 * (initial_energy - compute_total_energy(simulation.objects)) / initial_energy;

                std::cout << std::fixed << std::setprecision(2);

                std::cout << "time " << std::setw(5) << time << "  "
                          << "obj " << std::setw(2) << i << "  "
                          << "pos: (" << std::setw(7) << obj.position.x << ", " << std::setw(7) << obj.position.y << ")  "
                          << "vel: (" << std::setw(7) << obj.velocity.x << ", " << std::setw(7) << obj.velocity.y << ")  "
                          << "energy drift: " << std::setw(5) << drift << "%" << std::endl;
            }

            simulation.step(dt);
            
        }

        if (steps % print_buffer == 0) { std::cout << "fps: " << fps << std::endl; }

        steps += 1;
        time += dt;
    }

    return 0;
}

