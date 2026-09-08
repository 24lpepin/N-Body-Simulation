#include <queue>
#include "geometry.h"
#include "object.h"
#include "const.h"
#include <./SFML/Graphics.hpp>
#include "renderer.h"

inline std::unordered_map<std::string, sf::Color> COLOR_MAP = {
    {"red", sf::Color::Red}, 
    {"green", sf::Color::Green}, 
    {"blue", sf::Color::Blue}, 
    {"yellow", sf::Color::Yellow}, 
    {"magenta", sf::Color::Magenta},
    {"cyan", sf::Color::Cyan}, 
    {"white", sf::Color::White},
};

Renderer::Renderer(sf::RenderWindow& window): window(window) {}

void Renderer::draw_object(const Object& object) {
    // const double radius = 10.f * (object.mass / 2000 * std::pow(10, -15)) + 2.5; // Radius depends on mass of object
    const double radius = 10.f;
    sf::CircleShape shape(radius);
    sf::Vector2f _position = world_to_screen(vector_to_sfml(object.position));
    Vector2D position = Vector2D(_position.x, _position.y);
    position = position - Vector2D(radius, radius); // Subtract radius to center object
    shape.setPosition(vector_to_sfml(position)); 
    shape.setFillColor(get_color(object));
    window.draw(shape);
}

void Renderer::draw_objects(const std::vector<Object>& objects) {
    for (auto& obj : objects) {
        draw_object(obj);
    }
}
void Renderer::draw_path(const Object& object) {
    const std::deque<Vector2D>& path = object.path;
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        lines[i].position = world_to_screen(vector_to_sfml(path[i]));
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

sf::Vector2f Renderer::world_to_screen(const sf::Vector2f& v) {
    return static_cast<float>(DISTANCE_TO_PIXEL_CONVERSION) * v + sf::Vector2f(400, 400);
}

sf::Color Renderer::get_color(const Object& object) {
    if (auto iter = COLOR_MAP.find(object.color); iter != COLOR_MAP.end()) {
        return iter->second;
    }
    return COLOR_MAP.at("white"); // default
}

  sf::Vector2f Renderer::vector_to_sfml(const Vector2D& v) {return sf::Vector2f(v.x, v.y);};
