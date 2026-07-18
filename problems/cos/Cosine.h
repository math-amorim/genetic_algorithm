#pragma once
#include "../Problem.h"
#include <vector>
#include <cmath>
using namespace std;

class CosineProblem : public Problem {
private:
    double lowerBound;
    double upperBound;
    int bits;
    bool maximize;

public:
    CosineProblem(double lb, double ub, int b, bool max) 
        : lowerBound(lb), upperBound(ub), bits(b), maximize(max) {}

    double decode(const vector<int>& chrom) {
        int val = 0;
        for (int bit : chrom) {
            val = val * 2 + bit;
        }
        double range = upperBound - lowerBound;
        double steps = pow(2, bits) - 1;
        return lowerBound + (val / steps) * range;
    }

    double f(double x) {
        return cos(20 * x) - (abs(x) / 2.0) + (pow(x, 3) / 4.0);
    }

    void evaluate(Individual& ind) override {
        double x = decode(ind.chromosome);
        double fx = f(x);

        ind.properties["x"] = x;
        ind.properties["f(x)"] = fx;

        if (maximize) {
            ind.fitness = 4.0 + fx;
        } else {
            ind.fitness = 2.0 - fx;
        }
    }
};
