#include <iostream>
#include <cmath>
#include <vector>

#include "geometry.h"
#include "object.h"

#include <./SFML/Graphics.hpp>

int main()
{
    std::cout << "running";
    sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
    window.setPosition({100,100});
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}

// int main() {
//     std::vector<Object> objects;

//     Vector2D x(0,0);
//     Vector2D v(1,1);
//     Vector2D a(0,0);
//     double m = 1;
//     Object o(x,v,a,m);

//     objects.push_back(o);
//     std::cout << o << std::endl;
//     o.step(objects, 0.05);
//     std::cout << o << std::endl;
//     return 0;
// }
