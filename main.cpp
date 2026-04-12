#include <iostream>
#include <cmath>
#include <vector>

#include "geometry.h"
#include "object.h"

#include <./SFML/Graphics.hpp>

void draw_object(sf::RenderWindow& window, const Object& object) {
    sf::CircleShape shape(10.f);
    // std::cout << object.position << object.velocity << object.acceleration << std::endl;
    shape.setPosition(object.position);
    shape.setFillColor(sf::Color::White);
    window.draw(shape);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Orbital Simulation");
    window.setPosition({100,100});
    window.setFramerateLimit(60);
    
    const float dt = 0.025f;
    double m = std::pow(10, 16);

    Object object1({400, 300}, {70, 0}, {0, 0}, m / 10000);
    Object object2({400, 550}, {-60, 0}, {0, 0}, m / 100);
    Object object3({400, 400}, {00, 0}, {0, 0}, m);
    std::vector<Object> objects;
    objects.push_back(object1);
    objects.push_back(object2);
    objects.push_back(object3);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        std::vector<Object> next = objects;
        for (int i = 0; i < objects.size(); i++) {
            draw_object(window, next[i]);
            std::cout << "obj " << i << " pos: " << next[i].position << " vel: " << next[i].velocity << std::endl;
            next[i].step(objects, dt);
        }
        objects = next;
        window.display();
    }

    return 0;
}

