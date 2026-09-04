#include "barnes_hut_calculator.h"
#include "../object.h"
#include "../geometry.h"
#include "../const.h"

#include <algorithm>

BarnesHutCalculator::BarnesHutCalculator(double theta): theta(theta) {}

void BarnesHutCalculator::update_accelerations(std::vector<Object>& objects) {
    if (objects.empty()) {
        return;
    }

    build_tree(objects);

    // update accelerations
    for (Object& object : objects) {
        object.acceleration = calculate_acceleration(root.get(), object);
    }

    root = nullptr;
    return;
}

Vector2D BarnesHutCalculator::calculate_acceleration(Node* node, Object& object) {
    if (is_empty_node(node)) {
        return Vector2D(0, 0);
    }

    else if (is_external_node(node)) {
        if (node->body == &object) {
            return Vector2D(0, 0);
        }

        Vector2D direction = node->body->position - object.position;
        double distance = direction.magnitude();

        // Avoid division by zero
        if (distance == 0.0) {
            return Vector2D(0, 0);
        }

        return G * node->body->mass * direction /
               (distance * distance * distance);
    }

    else { // internal node
        Vector2D direction = node->center_of_mass - object.position;
        double distance = direction.magnitude();

        if (distance == 0.0) {
            return Vector2D(0, 0);
        }

        double width = node->bounds.x_max - node->bounds.x_min;

        // Far enough to treat as one body
        if (width / distance < theta) {
            return G * node->mass * direction /
                (distance * distance * distance);
        }

        // Recursively traverse children
        Vector2D acceleration(0, 0);

        for (const auto& child : node->children) {
            acceleration = acceleration + calculate_acceleration(child.get(), object);
        }

        return acceleration;
    }
}

void BarnesHutCalculator::build_tree(std::vector<Object>& objects) {
    if (root == nullptr) {
        root = std::make_unique<Node>();
        root->bounds = calculate_bounds(objects);
    }

    for (Object& obj : objects) {
        insert_object(root.get(), obj);
    }
}

void BarnesHutCalculator::insert_object(Node* node, Object& object) {
    if (is_empty_node(node)) {
        node->body = &object;
        node->center_of_mass = object.position;
        node->mass = object.mass;
    }
    else if (is_external_node(node)) {
        // Update center of mass
        update_center_of_mass(node, object);

        // Make children
        make_children(node);

        // Clean up current node's object (set to nullptr)
        Object* old_body = node->body;
        node->body = nullptr;

        // Move node's old object to appropriate quadrant
        Node* quadrant_old = find_quadrant(node, *(old_body));
        insert_object(quadrant_old, *(old_body));

        // Move new object to appropriate quadrant
        Node* quadrant_new = find_quadrant(node, object);
        insert_object(quadrant_new, object);
    }
    else if (is_internal_node(node)) {
        // Update center of mass
        update_center_of_mass(node, object);

        // Call insert object on appropriate quadrant
        Node* quadrant = find_quadrant(node, object);
        insert_object(quadrant, object);
    }
}

void BarnesHutCalculator::make_children(Node* node) {
    double x_mid = (node->bounds.x_min + node->bounds.x_max) / 2.0;
    double y_mid = (node->bounds.y_min + node->bounds.y_max) / 2.0;

    // NW
    node->children[0] = std::make_unique<Node>();
    node->children[0]->bounds = Bounds{
        node->bounds.x_min, y_mid,
        x_mid, node->bounds.y_max
    };

    // NE
    node->children[1] = std::make_unique<Node>();
    node->children[1]->bounds = Bounds{
        x_mid, y_mid,
        node->bounds.x_max, node->bounds.y_max
    };

    // SW
    node->children[2] = std::make_unique<Node>();
    node->children[2]->bounds = Bounds{
        node->bounds.x_min, node->bounds.y_min,
        x_mid, y_mid
    };

    // SE
    node->children[3] = std::make_unique<Node>();
    node->children[3]->bounds = Bounds{
        x_mid, node->bounds.y_min,
        node->bounds.x_max, y_mid
    };
}

void BarnesHutCalculator::update_center_of_mass(Node* node, Object& object) {
    double new_mass = node->mass + object.mass;
    Vector2D new_center_of_mass = (node->mass * node->center_of_mass + 
        object.mass * object.position) / (new_mass);
    node->mass = new_mass;
    node->center_of_mass = new_center_of_mass;
}

BarnesHutCalculator::Node* BarnesHutCalculator::find_quadrant(Node* node, Object& object) {
    Vector2D mp = Vector2D((node->bounds.x_max + node->bounds.x_min) / 2, 
                           (node->bounds.y_max + node->bounds.y_min) / 2);
    Vector2D pos = object.position;
    if (pos.x < mp.x && pos.y >= mp.y) return node->children[0].get();
    else if (pos.x >= mp.x && pos.y >= mp.y) return node->children[1].get();
    else if (pos.x < mp.x && pos.y < mp.y) return node->children[2].get();
    else if (pos.x >= mp.x && pos.y < mp.y) return node->children[3].get();
    else return nullptr;
}

bool BarnesHutCalculator::is_empty_node(Node* node) {
    // No body and no children
    return (node->body == nullptr && 
        std::all_of(node->children.begin(), node->children.end(), [](const auto& child){
            return child == nullptr;
        })
    );
}

bool BarnesHutCalculator::is_external_node(Node* node) {
    return (node->body != nullptr);
}

bool BarnesHutCalculator::is_internal_node(Node* node) {
    // No body but contains children
    return (node->body == nullptr && 
        std::all_of(node->children.begin(), node->children.end(), [](const auto& child){
            return child != nullptr;
        })
    );
}

BarnesHutCalculator::Bounds BarnesHutCalculator::calculate_bounds(const std::vector<Object>& objects) {
    double x_min = objects[0].position.x;
    double y_min = objects[0].position.y;
    double x_max = objects[0].position.x;
    double y_max = objects[0].position.y;
    for (const Object& obj : objects) {
        x_min = std::min(obj.position.x, x_min);
        y_min = std::min(obj.position.y, y_min);
        x_max = std::max(obj.position.x, x_max);
        y_max = std::max(obj.position.y, y_max);
    }
    return {.x_min=x_min, .y_min=y_min, .x_max=x_max, .y_max=y_max};
}