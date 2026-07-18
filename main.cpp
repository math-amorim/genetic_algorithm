#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

const int BITS_COSINE = 16;
const int BITS_RADIO = 10;
const int BITS_3SAT = 100;
const int POP_SIZE = 10;
const int POP_SIZE_NQUEENS = 30;
const int GENERATIONS = 10000;
const double CROSSOVER_RATE = 0.9;
const double MUTATION_RATE = 0.01;
const int TOURNAMENT_SIZE = 2;
const int NUM_RUNS = 10;
const int BASE_SEED = 42;
const int ELITE_COUNT = 1;

#include "src/Runner.h"
#include "problems/cos/Cosine.h"
#include "problems/radio/Radio.h"
#include "problems/3sat/3SAT.h"
#include "problems/nqueens/NQueens.h"
#include "problems/nqueens/NQueensValued.h"

void runCosine(vector<RunResult>& all, bool maximize) {
    string mode = maximize ? "max" : "min";
    CosineProblem problem(-2, 2, BITS_COSINE, maximize);

    RunConfig cfg;
    cfg.name = maximize ? "Funcao Máxima" : "Funcao Mínima";
    cfg.groupName = "Problema Função Cosseno";
    cfg.outputDir = "results/cosine/";
    cfg.csvName = mode + ".csv";
    cfg.popSize = POP_SIZE;
    cfg.createIndividual = []() { return randomIndividual(BITS_COSINE); };
    cfg.mutation = bitFlipMutation;
    cfg.crossover = onePointCrossover;
    cfg.selection = rouletteWithoutReplacement;

    all.push_back(run(problem, cfg));
}

void runRadio(vector<RunResult>& all) {
    RadioProblem problem;

    RunConfig cfg;
    cfg.name = "Rádios";
    cfg.groupName = "Problema Rádios";
    cfg.outputDir = "results/radio/";
    cfg.popSize = POP_SIZE;
    cfg.createIndividual = []() { return randomIndividual(BITS_RADIO); };
    cfg.mutation = bitFlipMutation;
    cfg.crossover = onePointCrossover;
    cfg.selection = rouletteWithoutReplacement;


    all.push_back(run(problem, cfg));
}

void run3SAT(vector<RunResult>& all) {
    ThreeSATProblem problem("problems/3sat/clausulas.txt");

    RunConfig cfg;
    cfg.name = "3-SAT";
    cfg.groupName = "Problema 3-SAT";
    cfg.outputDir = "results/3sat/";
    cfg.popSize = POP_SIZE;
    cfg.createIndividual = []() { return randomIndividual(BITS_3SAT); };
    cfg.mutation = bitFlipMutation;
    cfg.crossover = onePointCrossover;
    cfg.selection = rouletteWithoutReplacement;


    all.push_back(run(problem, cfg));
}

void runNQueensValued(vector<RunResult>& all) {
    vector<int> sizes = {8, 16, 32, 64, 128};

    for (int n : sizes) {
        NQueensValued problem(n);

        RunConfig cfg;
        cfg.name = "N-Rainhas Valorado (N=" + to_string(n) + ")";
        cfg.groupName = "N-Rainhas Valorado";
        cfg.outputDir = "results/nqueens_valued/";
        cfg.csvName = to_string(n) + ".csv";
        cfg.popSize = POP_SIZE_NQUEENS;
        cfg.isPermutation = true;
        cfg.createIndividual = [n]() { return randomPermutation(n); };
        cfg.crossover = oxCrossover;
        cfg.mutation = swapMutation;
        cfg.selection = [](const vector<Individual>& pop) { return tournamentSelection(pop, TOURNAMENT_SIZE); };

        all.push_back(run(problem, cfg));
    }
}

void runNQueens(vector<RunResult>& all) {
    vector<int> sizes = {8, 16, 32, 64, 128};

    for (int n : sizes) {
        NQueens problem(n);

        RunConfig cfg;
        cfg.name = "N-Rainhas Normal (N=" + to_string(n) + ")";
        cfg.groupName = "N-Rainhas Normal";
        cfg.outputDir = "results/nqueens_normal/";
        cfg.csvName = to_string(n) + ".csv";
        cfg.popSize = POP_SIZE_NQUEENS;
        cfg.isPermutation = true;
        cfg.createIndividual = [n]() { return randomPermutation(n); };
        cfg.crossover = oxCrossover;
        cfg.mutation = swapMutation;
        cfg.selection = [](const vector<Individual>& pop) { return tournamentSelection(pop, TOURNAMENT_SIZE); };

        all.push_back(run(problem, cfg));
    } 

}

int main() {
    cout << "\n===========================\n";
    cout << "     GENETIC ALGORITHM\n";
    cout << "===========================\n";
    cout << "[1] Funcao Cosseno\n";
    cout << "[2] Radios\n";
    cout << "[3] 3-SAT\n";
    cout << "[4] N-Rainhas\n";
    cout << "[5] N-Rainhas Valorado\n";
    cout << "[6] Todos\n";
    cout << "===========================\n";
    cout << "Escolha: ";

    int choice;
    cin >> choice;

    vector<RunResult> allResults;

    switch (choice) {
        case 1:
            runCosine(allResults, true);
            runCosine(allResults, false);
            break;
        case 2: runRadio(allResults); break;
        case 3: run3SAT(allResults); break;
        case 4: runNQueens(allResults); break;
        case 5: runNQueensValued(allResults); break;
        case 6:
            runCosine(allResults, true);
            runCosine(allResults, false);
            runRadio(allResults);
            run3SAT(allResults);
            runNQueens(allResults);
            runNQueensValued(allResults);
            break;
    }

    if (!allResults.empty()) {
        makeDir("results/");
        printStatsTable(allResults);

        map<string, vector<RunResult>> groups;
        for (const auto& r : allResults)
            groups[r.group].push_back(r);

        for (const auto& [group, results] : groups) {
            string dir = results[0].outputDir;
            saveGroupBoxplot(results, dir, group);
        }
    }
    return 0;
}
