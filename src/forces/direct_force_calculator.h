#pragma once

#include "force_calculator.h"

#include <vector>

class DirectForceCalculator : public ForceCalculator {
public:
    void update_accelerations(std::vector<Object>& objects) override;
};
