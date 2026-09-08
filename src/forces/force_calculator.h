#pragma once

#include <memory>
#include <array>

#include "../object.h"

class ForceCalculator {
public:
    virtual ~ForceCalculator() = default;
    virtual void update_accelerations(
        std::vector<Object>& objects
    ) = 0;
};
