#pragma once
#include "Individual.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <cstdlib>
using namespace std;

inline Individual randomIndividual(int bits) {
    Individual ind;
    ind.chromosome.resize(bits);
    for (int i = 0; i < bits; i++)
        ind.chromosome[i] = rand() % 2;
    return ind;
}

inline vector<Individual> createPopulation(int size, int bits) {
    vector<Individual> pop;
    for (int i = 0; i < size; i++)
        pop.push_back(randomIndividual(bits));
    return pop;
}

inline vector<Individual> createPopulation(int size, function<Individual()> factory) {
    vector<Individual> pop;
    for (int i = 0; i < size; i++)
        pop.push_back(factory());
    return pop;
}

inline Individual randomPermutation(int n) {
    Individual ind;
    ind.chromosome.resize(n);
    for (int i = 0; i < n; i++)
        ind.chromosome[i] = i;
    
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(ind.chromosome[i], ind.chromosome[j]);
    }
    return ind;
}