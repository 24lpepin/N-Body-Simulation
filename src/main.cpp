#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <queue>

#include "geometry.h"
#include "object.h"
#include "const.h"
#include "simulation.h"
#include "renderer.h"

#include <./SFML/Graphics.hpp>

std::vector<Object> create_objects(int n = 1) {
    double m = std::pow(10, 16);

    switch (n) 
    {
        case 1:
            return std::vector<Object> {
                {{0, 0}, {0, 0}, 1}, // sun
                {{1, 0}, {0, 6.28}, 3 * pow(10, -6)}, // earth
            };

        case 2: // Figure 8
            Vector2D position(0.97000436, -0.24308753);
            Vector2D velocity(0.4662036850, 0.4323657300);
            velocity = velocity * sqrt(G); // Figure 8 configuration uses G=1. We need to normalize for our G
            return std::vector<Object> {
                {position, velocity, 1}, 
                {-position, velocity, 1}, 
                {{0,0}, -2 * velocity, 1},
            };

        // case 3: // 4 bodies
        //     Vector2D position1(1.7, -1);
        //     Vector2D position2(-1.2, 0.5);
        //     Vector2D velocity1(0.5, 0.5);
        //     Vector2D velocity2(-0.2, 1.1);
        //     velocity1 = velocity1 * sqrt(G); 
        //     velocity2 = velocity2 * sqrt(G);
        //     return std::vector<Object> {
        //         {-position1, -2 * velocity1, 1}, 
        //         {position1, velocity2, 1}, 
        //         {{0,0}, -2 * velocity2, 1},
        //         {position2, velocity1, 1},
        //     };
    }

    return std::vector<Object>{};
    // return std::vector<Object> {object1, object2, object3};
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

    // 100 pixels = 1 au
    // (400, 400) -> (0,0)
    // (500, 400) -> (1,0)
    // Earth: x = (1, 0), v = (6.28, 0)
    
    const double dt = 0.00002f;
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
                    window.clear();
                    break;

                case sf::Keyboard::Key::R: // Reset configuration
                    simulation.clear();
                    window.clear();
                    objs = simulation.add_objects(create_objects(n));
                    for (auto& obj : objs) {
                        renderer.draw_object(obj);
                    }
                    initial_energy = compute_total_energy(simulation.objects); // recompute initial energy
                    break;

                case sf::Keyboard::Key::B: // Adds a new object in a random location
                    Object object = simulation.add_random_object();
                    initial_energy = compute_total_energy(simulation.objects); // recompute initial energy
                    renderer.draw_object(object);
                    break;
                }
            }
        }

        window.display(); // redraw window here incase window is paused.

        if (paused) {
            continue;
        }

        if (steps % draw_buffer == 0) { window.clear(); }
        
        for (int i = 0; i < simulation.objects.size(); i++) {
            Object& obj = simulation.objects[i];

            if (steps % draw_buffer== 0) {
                simulation.update_paths();
                renderer.draw_object(obj);
                renderer.draw_path(obj.path);
            }

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

