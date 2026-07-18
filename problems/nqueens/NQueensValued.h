#pragma once
#include "../Problem.h"
#include <vector>
#include <cmath>
using namespace std;

class NQueensValued : public Problem {
private:
    vector<vector<double>> board;
    int n;

public:
    NQueensValued(int boardSize) : n(boardSize) {
        board.resize(n, vector<double>(n));

        int value = 1; 

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if(i % 2 == 0) board[i][j] = sqrt(value);
                else board[i][j] = log(value);

                value++;
            }
        }
    }

    void evaluate(Individual& ind) override {
        int attacks = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if(abs(ind.chromosome[i] - ind.chromosome[j]) == abs(i - j))
                    attacks++;
            }
        }

        double valueSum = 0.0;
        
        for(int i = 0; i < n; i++) {
            int j = ind.chromosome[i];
            valueSum += board[i][j];
        }

    
        double penalty = valueSum * 10;
        ind.fitness = valueSum - attacks * penalty;        
        ind.properties["attacks"] = attacks;
        ind.properties["feasible"] = (attacks == 0); 
    }
};
