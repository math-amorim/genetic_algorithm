#pragma once
#include "../src/Individual.h"

class Problem {
public:
    virtual void evaluate(Individual& ind) = 0;
    virtual ~Problem() = default;
};