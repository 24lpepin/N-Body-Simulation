#include <queue>
#include "geometry.h"
#include "object.h"
#include <./SFML/Graphics.hpp>

class Renderer {
public:
    Renderer(sf::RenderWindow& window);

    void draw_object(const Object& object);
    void draw_objects(const std::vector<Object>& objects);
    void draw_path(const Object& object);
    void draw_paths(const std::vector<Object>& objects);
    void clear();
    void display();
    void close();

private:
    sf::RenderWindow& window;

    Vector2D world_to_screen(const sf::Vector2f& v); // Converts given position in au to pixels

    sf::Color get_color(const Object& object);
};