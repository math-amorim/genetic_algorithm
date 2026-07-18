#pragma once
#include "../Problem.h"
#include <vector>
using namespace std;

class NQueens : public Problem {
private:
    int n;

public:
    NQueens(int boardSize) : n(boardSize) {}

    void evaluate(Individual& ind) override {
        int attacks = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (abs(ind.chromosome[i] - ind.chromosome[j]) == abs(i - j))
                    attacks++;
            }
        }
        
        int maxPairs = n * (n - 1) / 2;
        
        ind.fitness = maxPairs - attacks;        
        ind.properties["attacks"] = attacks;
    }
};
