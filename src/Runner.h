#pragma once
#include "GA.h"
#include "Utils.h"
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

struct RunResult {
    string name;
    string group;
    string outputDir;
    vector<double> bestFitnesses;
    Individual bestIndividual;
};

struct RunConfig {
    string name;
    string groupName;
    string outputDir;
    string csvName = "convergence.csv";
    int popSize;
    int generations = GENERATIONS;
    double crossoverRate = CROSSOVER_RATE;
    double mutationRate = MUTATION_RATE;
    int numRuns = NUM_RUNS;
    int baseSeed = BASE_SEED;
    int eliteCount = ELITE_COUNT;
    int tournamentSize = TOURNAMENT_SIZE;
    function<Individual()> createIndividual;
    function<void(Individual&, Individual&)> crossover;
    function<void(Individual&, double)> mutation;
    function<vector<Individual>(const vector<Individual>&)> selection;
    bool isPermutation = false;
};

inline void makeDir(const string& path) {
    string cmd = "mkdir -p \"" + path + "\"";
    system(cmd.c_str());
}

inline void saveGraph(const string& csvPath, const string& title) {
    string cmd = "python3 scripts/plot.py \"" + csvPath + "\" \"" + title + "\"";
    system(cmd.c_str());
}

inline void saveBoxplot(const string& summaryPath, const string& title) {
    string cmd = "python3 scripts/plot.py --boxplot \"" + summaryPath + "\" \"" + title + "\"";
    system(cmd.c_str());
}

inline void saveGroupBoxplot(const vector<RunResult>& group, const string& outputDir, const string& title) {
    string csvPath = outputDir + "summary.csv";
    ofstream f(csvPath);
    f << "problem,run,best_fitness\n";
    for (const auto& r : group)
        for (size_t i = 0; i < r.bestFitnesses.size(); i++)
            f << r.name << "," << (i + 1) << "," << r.bestFitnesses[i] << "\n";
    f.close();
    saveBoxplot(csvPath, title);
}

inline Individual bestIndividual(const vector<Individual>& pop) {
    return *max_element(pop.begin(), pop.end(),
        [](const Individual& a, const Individual& b) { return a.fitness < b.fitness; });
}

inline string formatChromosome(const Individual& ind, bool permutation = false) {
    if (permutation) {
        string r = "[";
        for (size_t i = 0; i < ind.chromosome.size(); i++) {
            if (i > 0) r += ", ";
            r += to_string(ind.chromosome[i]);
        }
        return r + "]";
    }
    string b;
    for (int v : ind.chromosome) b += to_string(v);
    return b;
}

inline void printIndividual(const Individual& ind, const string& label, bool permutation = false) {
    cout << label << "\n";
    cout << "Cromossomo : " << formatChromosome(ind, permutation) << "\n";
    cout << "Fitness    : " << ind.fitness << "\n";
    for (const auto& [k, v] : ind.properties)
        cout << left << setw(12) << k << ": " << v << "\n";
    cout << "\n";
}

inline void computeStats(const vector<double>& values, double& mean, double& stddev, double& bst, double& wst) {
    double sum = 0.0;
    for (double v : values) sum += v;
    mean = sum / values.size();

    double sq = 0.0;
    for (double v : values) sq += (v - mean) * (v - mean);
    stddev = sqrt(sq / values.size());

    bst = *max_element(values.begin(), values.end());
    wst = *min_element(values.begin(), values.end());
}

inline void printStatsTable(const vector<RunResult>& results, int numRuns = NUM_RUNS) {
    cout << "\n" << string(72, '=') << "\n";
    cout << "        RESULTADOS FINAIS (" << numRuns << " EXECUCOES)\n";
    cout << string(72, '=') << "\n";
    cout << left
         << setw(30) << "Problema"
         << setw(12) << "Media"
         << setw(12) << "Desvio"
         << setw(12) << "Melhor"
         << setw(12) << "Pior" << "\n";
    cout << string(72, '-') << "\n";

    for (const auto& r : results) {
        double mean, stddev, bst, wst;
        computeStats(r.bestFitnesses, mean, stddev, bst, wst);
        cout << left
             << setw(30) << r.name
             << setw(12) << fixed << setprecision(4) << mean
             << setw(12) << stddev
             << setw(12) << bst
             << setw(12) << wst << "\n";
    }
    cout << string(72, '=') << "\n";
}

inline RunResult run(Problem& problem, const RunConfig& cfg) {
    makeDir(cfg.outputDir);

    vector<double> bestFitnesses;
    Individual overallBest;
    bool first = true;
    cout << "\nRodando problema " << cfg.name << "...\n";
    cout << "\n" << endl; 
    for (int r = 1; r <= cfg.numRuns; r++) {
        srand(cfg.baseSeed + r);

        auto pop = createPopulation(cfg.popSize, cfg.createIndividual);
        for (auto& ind : pop) problem.evaluate(ind);

        cout << "Rodando run " << r << " do problema " << cfg.name << "...\n";

        GA ga;
        ga.problemName = cfg.groupName;
        ga.populationSize = cfg.popSize;
        ga.generations = cfg.generations;
        ga.tournamentSize = cfg.tournamentSize;
        ga.crossoverRate = cfg.crossoverRate;
        ga.mutationRate = cfg.mutationRate;
        ga.eliteCount = cfg.eliteCount;
        ga.logFile = cfg.outputDir + cfg.csvName;
        ga.crossover = cfg.crossover;
        ga.mutation = cfg.mutation;
        ga.selection = cfg.selection;

        ga.run(problem, pop);

        Individual best = bestIndividual(pop);
        bestFitnesses.push_back(best.fitness);

        if (first || best.fitness > overallBest.fitness) {
            overallBest = best;
            first = false;
        }
    }

    saveGraph(cfg.outputDir + cfg.csvName, cfg.name);
    cout << "\n========================================\n";
    cout << "Resultado Final - " << cfg.name << "\n";
    cout << "========================================\n";
    printIndividual(overallBest, "Melhor Individuo", cfg.isPermutation);
    string group = cfg.groupName.empty() ? cfg.name : cfg.groupName;
    return {cfg.name, group, cfg.outputDir, bestFitnesses, overallBest};
}
