#pragma once

#include "object.h"

#include <vector>

class ObjectLoader {
public:
    std::vector<Object> create_objects(int n = 1, unsigned seed = 42);
    std::vector<Object> load_objects(int id = 1);
};