#include <queue>
#include "geometry.h"
#include "object.h"
#include "const.h"
#include <./SFML/Graphics.hpp>
#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window): window(window) {}

void Renderer::draw_object(const Object& object) {
    // const double radius = 10.f * (object.mass / 2000 * std::pow(10, -15)) + 2.5; // Radius depends on mass of object
    const double radius = 10.f;
    sf::CircleShape shape(radius);
    // std::cout << object.position << object.velocity << object.acceleration << std::endl;
    const Vector2D position = 100 * (object.position) - Vector2D(radius, radius) + Vector2D(400, 400);
    shape.setPosition(position); // Subtract radius to center object
    shape.setFillColor(sf::Color::White);
    window.draw(shape);
}

void Renderer::draw_objects(const std::vector<Object>& objects) {
    for (auto& obj : objects) {
        draw_object(obj);
    }
}
void Renderer::draw_path(const std::deque<sf::Vector2f>& path) {
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        lines[i].position = path[i];
        lines[i].color = sf::Color(255, 255, 255, i * 255 / path.size());
    }
    window.draw(lines);
}

void Renderer::clear() {
    window.clear();

}

void Renderer::display() {
    window.display();
}

void Renderer::close() {
    window.close();
}

sf::Vector2f Renderer::world_to_screen(const Vector2D& v) {
    return DISTANCE_TO_PIXEL_CONVERSION * v;
}