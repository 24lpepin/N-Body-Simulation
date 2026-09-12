#include <iostream>
#include <iomanip>
#include <vector>

#include "object.h"
#include "object_loader.h"
#include "simulation.h"
#include "renderer.h"
#include "forces/direct_force_calculator.h"
#include "forces/barnes_hut_calculator.h"

#include <./SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Orbital Simulation");
    window.setPosition({100,100});
    window.setVerticalSyncEnabled(true);

    Renderer renderer(window);

    bool paused = false;
    
    double dt = 0.00002;
    double time = 0.0;
    int steps = 0;
    const int draw_buffer = 10;
    const int print_buffer = draw_buffer * 200;
    
    const int id = 3;
    // Simulation simulation(std::make_unique<DirectForceCalculator>(), create_objects(n));
    ObjectLoader object_loader;
    Simulation simulation(std::make_unique<BarnesHutCalculator>(), object_loader.load_objects(id));

    sf::Clock clock;

    double initial_energy = simulation.compute_total_energy();
    double initial_angular_momentum = simulation.compute_total_angular_momentum();

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float steps_per_sec = 1.0f / elapsed.asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                const sf::Keyboard::Key code = event->getIf<sf::Event::KeyPressed>()->code;

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
                {
                    simulation.clear();
                    renderer.clear();
                    std::vector<Object> objs = simulation.add_objects(object_loader.load_objects(id));
                    renderer.draw(objs);
                    initial_energy = simulation.compute_total_energy(); // recompute initial energy
                    initial_angular_momentum = simulation.compute_total_angular_momentum();
                    break;
                }

                case sf::Keyboard::Key::B: // Adds a new object in a random location
                {
                    Object object = simulation.add_random_object();
                    initial_energy = simulation.compute_total_energy(); // recompute initial energy
                    initial_angular_momentum = simulation.compute_total_angular_momentum();
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
        
        simulation.step(dt);
        for (int i = 0; i < simulation.objects.size(); i++) {
            Object& obj = simulation.objects[i];

            if (steps % print_buffer == 0) {

                std::cout << std::fixed << std::setprecision(2);

                std::cout << "time " << std::setw(5) << time << "  "
                          << "obj " << std::setw(2) << i << "  "
                          << "pos: (" << std::setw(7) << obj.position.x << ", " << std::setw(7) << obj.position.y << ")  "
                          << "vel: (" << std::setw(7) << obj.velocity.x << ", " << std::setw(7) << obj.velocity.y << ")  " << std::endl;
            }
        }

        if (steps % print_buffer == 0) { 

            double energy_drift = 100 * (initial_energy - simulation.compute_total_energy()) / initial_energy;
            double angular_momentum_drift = 100 * (initial_angular_momentum - simulation.compute_total_angular_momentum()) / initial_angular_momentum;

            std::cout << "steps/sec: " << steps_per_sec << "  "
                      << "energy drift: " << std::scientific << std::setw(5) << energy_drift << "% " 
                      << "angular momentum drift: " << std::scientific << std::setw(5) << angular_momentum_drift << "% " << std::endl; 
        }

        steps += 1;
        time += dt;
    }

    return 0;
}

