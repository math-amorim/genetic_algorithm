#pragma once
#include "Individual.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

inline void onePointCrossover(Individual& a, Individual& b) {
    int size = a.chromosome.size();
    int point = rand() % size;
    for (int i = point; i < size; i++)
        swap(a.chromosome[i], b.chromosome[i]);
}

inline void bitFlipMutation(Individual& ind, double prob) {
    for (int i = 0; i < (int)ind.chromosome.size(); i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < prob)
            ind.chromosome[i] = 1 - ind.chromosome[i];
    }
}

inline void oxCrossover(Individual& a, Individual& b) {
    int n = a.chromosome.size();
    int p1 = rand() % n;
    int p2 = rand() % n;

    if (p1 > p2)
        swap(p1, p2);

    vector<int> parentA = a.chromosome;
    vector<int> parentB = b.chromosome;

    vector<int> childA(n, -1);
    vector<int> childB(n, -1);

    for (int i = p1; i <= p2; i++) {
        childA[i] = parentA[i];
        childB[i] = parentB[i];
    }

    int pos = (p2 + 1) % n;

    for (int k = 0; k < n; k++) {
        int gene = parentB[(p2 + 1 + k) % n];

        if (find(childA.begin(), childA.end(), gene) == childA.end()) {
            childA[pos] = gene;
            pos = (pos + 1) % n;
        }
    }

    pos = (p2 + 1) % n;

    for (int k = 0; k < n; k++) {
        int gene = parentA[(p2 + 1 + k) % n];
        if (find(childB.begin(), childB.end(), gene) == childB.end()) {
            childB[pos] = gene;
            pos = (pos + 1) % n;
        }
    }

    a.chromosome = move(childA);
    b.chromosome = move(childB);
}


inline void swapMutation(Individual& ind, double prob) {
    int size = ind.chromosome.size();
    for (int i = 0; i < size; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < prob) {
            int j = rand() % size;
            swap(ind.chromosome[i], ind.chromosome[j]);
        }
    }
}
