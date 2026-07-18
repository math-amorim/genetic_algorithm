#pragma once
#include "Individual.h"
#include <cstdlib>
using namespace std;

inline vector<Individual> rouletteWithoutReplacement(const vector<Individual>& pop) {
    vector<Individual> pool = pop;
    vector<Individual> selected;
    selected.reserve(pop.size());

    while (!pool.empty()) {
        double totalFitness = 0.0;

        for (const auto& ind : pool) totalFitness += ind.fitness;

        double r = ((double) rand() / RAND_MAX) * totalFitness;

        double acc = 0.0;
        int chosen = 0;

        for (int i = 0; i < (int)pool.size(); i++) {
            acc += pool[i].fitness;
            if (acc >= r) {
                chosen = i;
                break;
            }
        }
        selected.push_back(pool[chosen]);
        pool.erase(pool.begin() + chosen);
    }
    return selected;
}

inline vector<Individual> tournamentSelection(const vector<Individual>& pop, int tournamentSize) {
    int N = pop.size();
    vector<Individual> selected;
    selected.reserve(N);

    for (int s = 0; s < N; s++) {
        int bestIdx = rand() % N;
        for (int i = 1; i < tournamentSize; i++) {
            int challenger = rand() % N;
            if (pop[challenger].fitness > pop[bestIdx].fitness) bestIdx = challenger;
        }
        selected.push_back(pop[bestIdx]);
    }

    return selected;
}