#pragma once
#include "../Problem.h"
using namespace std;

class RadioProblem : public Problem {
public:
    void evaluate(Individual& ind) override {

        int st_bin = 0;
        for (int i = 0; i < 5; i++)
            st_bin = st_bin * 2 + ind.chromosome[i];

        int lx_bin = 0;
        for (int i = 5; i < 10; i++)
            lx_bin = lx_bin * 2 + ind.chromosome[i];

        int ST = round((st_bin / 31.0) * 24);
        int LX = round((lx_bin / 31.0) * 16);

        // Função objetivo
        double FO = 30 * ST + 40 * LX;
        double FOn = FO / 1360.0;

        // Restrição
        double H = ST + 2 * LX - 40;
        double Hn = max(0.0, H / 16.0);

        // Fitness
        ind.fitness = max(0.0, FOn - Hn);
        ind.properties["ST"] = ST;
        ind.properties["LX"] = LX;
        ind.properties["f(x)"] = FO;
    }
};