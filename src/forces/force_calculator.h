#pragma once

#include "../object.h"

#include <vector>

class ForceCalculator {
public:
    virtual ~ForceCalculator() = default;
    virtual void update_accelerations(
        std::vector<Object>& objects
    ) = 0;
};
