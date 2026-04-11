#include <iostream>
#include <cmath>
#include <vector>

#include "geometry.h"
#include "object.h"

#include <./SFML/Graphics.hpp>

void draw_object(sf::RenderWindow& window, const Object& object) {
    sf::CircleShape shape(10.f);
    std::cout << object.position << std::endl;
    shape.setPosition(object.position);
    shape.setFillColor(sf::Color::White);
    window.draw(shape);
}

int main()
{
    std::cout << "running";
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Orbital Simulation");
    window.setPosition({100,100});
    window.setFramerateLimit(60);
    
    Vector2D position1(60, 60);
    Vector2D position2(140, 60);
    Vector2D velocity(0, 1);
    Vector2D acceleration(0, 0);
    double m = std::pow(10,9);
    Object object1(position1, velocity, acceleration, m);
    Object object2(position2, velocity, acceleration, m);
    std::vector<Object> objects;
    objects.push_back(object1);
    objects.push_back(object2);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        draw_object(window, object1);
        draw_object(window, object2);
        object1.step(objects, .05);
        object2.step(objects, .05);
        window.display();
    }

    return 0;
}

