#include "force_calculator.h"
#include "../object.h"

class BarnesHutCalculator : public ForceCalculator {
public:
    double theta;
    
    BarnesHutCalculator(double theta = 0.5);
    void update_accelerations(std::vector<Object>& objects) override;

private:
    struct Bounds
    {
        double x_min;
        double y_min;
        double x_max;
        double y_max;
    };

    struct Node
    {
        Bounds bounds;
        Object* body = nullptr;

        Vector2D center_of_mass = Vector2D(0, 0);
        double mass = 0.0;
        
        std::array<std::unique_ptr<Node>, 4> children; // NW, NE, SW, SE
    };

    std::unique_ptr<Node> root = nullptr;

    Vector2D calculate_acceleration(Node* node, Object& object);
    void build_tree(std::vector<Object>& objects);

    void insert_object(Node* node, Object& object);

    void make_children(Node* node);
    void update_center_of_mass(Node* node, Object& object);
    Node* find_quadrant(Node* node, Object& object);
    bool is_empty_node(Node* node);
    bool is_external_node(Node* node);
    bool is_internal_node(Node* node);
    
    Bounds calculate_bounds(const std::vector<Object>& objects);
    
};