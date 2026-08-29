#include "force_calculator.h"

class DirectForceCalculator : public ForceCalculator {
public:
    void update_accelerations(std::vector<Object>& objects) override;
};
