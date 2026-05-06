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
    Vector2D position = world_to_screen(object.position);
    position = position - Vector2D(radius, radius); // Subtract radius to center object
    shape.setPosition(position); 
    shape.setFillColor(get_color(object));
    window.draw(shape);
}

void Renderer::draw_objects(const std::vector<Object>& objects) {
    for (auto& obj : objects) {
        draw_object(obj);
    }
}
void Renderer::draw_path(const Object& object) {
    const std::deque<sf::Vector2f>& path = object.path;
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        lines[i].position = world_to_screen(path[i]);
        lines[i].color = get_color(object);
        lines[i].color.a = i * 255 / path.size();
        //lines[i].color = sf::Color(255, 255, 255, i * 255 / path.size());
    }
    window.draw(lines);
}

void Renderer::draw_paths(const std::vector<Object>& objects) {
    for (auto& obj : objects) {
        draw_path(obj);
    }
}

void Renderer::draw(const std::vector<Object>& objects) {
    draw_objects(objects);
    draw_paths(objects);
}

void Renderer::draw(const Object& object) {
    draw_object(object);
    draw_path(object);
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

Vector2D Renderer::world_to_screen(const sf::Vector2f& v) {
    return DISTANCE_TO_PIXEL_CONVERSION * Vector2D(v.x, v.y) + Vector2D(400, 400);
}

sf::Color Renderer::get_color(const Object& object) {
    return COLORS[object.id % COLORS.size()];
}