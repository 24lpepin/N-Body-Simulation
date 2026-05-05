#include <queue>
#include "geometry.h"
#include "object.h"
#include <./SFML/Graphics.hpp>

class Renderer {
public:
    Renderer(sf::RenderWindow& window);

    void draw_object(const Object& object);
    void draw_objects(const std::vector<Object>& objects);
    void draw_path(const std::deque<sf::Vector2f>& path);
    void clear();
    void display();
    void close();

private:
    sf::RenderWindow& window;

    sf::Vector2f world_to_screen(const Vector2D& v); // Converts given position in au to pixels
};