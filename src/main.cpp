#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <queue>

#include "geometry.h"
#include "object.h"
#include "const.h"
#include "simulation.h"

#include <./SFML/Graphics.hpp>

void draw_object(sf::RenderWindow& window, const Object& object) {
    const double radius = 10.f * (object.mass / 2000 * std::pow(10, -15)) + 2.5; // Radius depends on mass of object
    // const double radius = 10.f;
    sf::CircleShape shape(radius);
    // std::cout << object.position << object.velocity << object.acceleration << std::endl;
    shape.setPosition(object.position - Vector2D(radius, radius)); // Subtract radius to center object
    shape.setFillColor(sf::Color::White);
    window.draw(shape);
}

void draw_path(sf::RenderWindow& window, const std::deque<sf::Vector2f>& path) {
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        lines[i].position = path[i];
        lines[i].color = sf::Color(255, 255, 255, i * 255 / path.size());
    }
    window.draw(lines);
}

std::vector<Object> create_objects(int n = 1) {
    double m = std::pow(10, 16);

    // Object object1({400, 300}, {70, 0}, {0, 0}, m / 10000);
    // Object object2({400, 550}, {-60, 0}, {0, 0}, m / 100);
    // Object object3({400, 300}, {-80, 0}, {0, 0}, m / 10000);
    Object object3({400, 300}, {-80, 0}, {0, 0}, m);
    Object object1({400, 400}, {0, 0}, {0, 0}, m);
    // Object object2({400, 450}, {110, 0}, {0, 0}, m / 10000);
    Object object2({400, 450}, {110, 0}, {0, 0}, m);

    // return std::vector<Object> {object1, object2};
    return std::vector<Object> {object1, object2, object3};
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

    bool paused = false;
    
    const double dt = 0.0002f;
    double time = 0.0f;
    int steps = 0;
    const int draw_buffer = 10;
    const int print_buffer = draw_buffer * 200;
    
    Simulation simulation(create_objects());

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

                case sf::Keyboard::Key::B: // Adds a new object in a random location
                    Object object = simulation.add_random_object();
                    initial_energy = compute_total_energy(simulation.objects); // recompute initial energy
                    draw_object(window, object);
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
                draw_object(window, obj);
                draw_path(window, obj.path);
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

