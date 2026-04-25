#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

#include "geometry.h"
#include "object.h"
#include "const.h"

#include <./SFML/Graphics.hpp>

void draw_object(sf::RenderWindow& window, const Object& object) {
    const float radius = 10.f;
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

std::vector<Object> create_objects() {
    double m = std::pow(10, 16);

    Object object1({400, 300}, {70, 0}, {0, 0}, m / 10000);
    Object object2({400, 550}, {-60, 0}, {0, 0}, m / 100);
    Object object3({400, 400}, {00, 0}, {0, 0}, m);
    // Object object1({400, 400}, {0, 0}, {0, 0}, m);
    // Object object2({400, 500}, {70, 0}, {0, 0}, m / 1000);

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
    window.setFramerateLimit(60);

    bool paused = false;
    
    const float dt = 0.025f;
    
    std::vector<Object> objects = create_objects();

    std::vector<std::deque<sf::Vector2f>> paths(objects.size());

    const double initial_energy = compute_total_energy(objects);

    while (window.isOpen())
    {
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
                    objects.clear();
                    paths.clear();
                    window.clear();
                    break;

                case sf::Keyboard::Key::B: // Add new objects
                    std::vector<Object> new_objects = create_objects();
                    for (Object object : new_objects) {
                        objects.push_back(object);
                        paths.push_back(std::deque<sf::Vector2f>());
                        draw_object(window, object);
                    }
                    break;
                }
            }
        }

        window.display(); // redraw window here incase window is paused.

        if (paused) {
            continue;
        }

        window.clear();
        for (int i = 0; i < objects.size(); i++) {
            Object& obj = objects[i];

            std::deque<sf::Vector2f>& path = paths[i];
            path.push_back(obj.position);
            if (path.size() > MAX_PATH_LEN) {
                path.pop_front();
            }

            Vector2D force = obj.compute_force(objects);
            draw_object(window, obj);
            std::cout << "obj " << i 
                      << " pos: " << objects[i].position 
                      << " vel: " << objects[i].velocity 
                      << " energy drift: " << 100 * (initial_energy - compute_total_energy(objects)) / initial_energy << "%"
                      << std::endl;
            obj.step(objects, force, dt);

            draw_path(window, path);
            
        }
    }

    return 0;
}

