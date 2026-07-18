#pragma once
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include "../problems/Problem.h"
#include "Selection.h"
#include "Operators.h"
#include "Logger.h"
using namespace std;

class GA {
public:
    string problemName;
    int populationSize;
    int tournamentSize; 
    int generations;
    double crossoverRate;
    double mutationRate;
    int eliteCount;
    function<void(Individual&, Individual&)> crossover;
    function<void(Individual&, double)> mutation;
    function<vector<Individual>(const vector<Individual>&)> selection;
    string logFile = "convergence.csv";

    void run(Problem& problem, vector<Individual>& pop) {
        Logger logger(logFile);

        for (int g = 0; g < generations; g++) {

            for (auto& ind : pop)
                problem.evaluate(ind);

            int bestIdx = 0;
            double best = pop[0].fitness;
            double bestValue = (pop[0].properties.count("value") ? pop[0].properties["value"] : pop[0].fitness);
            double sum = 0.0;
            double sumValues = 0.0;

            for (size_t i = 0; i < pop.size(); i++) {
                double v = (pop[i].properties.count("value") ? pop[i].properties["value"] : pop[i].fitness);
                if (pop[i].fitness > best) {
                    best = pop[i].fitness;
                    bestIdx = i;
                    bestValue = v;
                }
                sum += pop[i].fitness;
                sumValues += v;
            }

            double avg = sum / pop.size();
            double avgValue = sumValues / pop.size();
            logger.log(g, best, avg, bestValue, avgValue);
          
            vector<Individual> parents = selection(pop); 
            
            for (size_t i = parents.size() - 1; i > 0; i--) {
                int j = rand() % (i + 1);
                swap(parents[i], parents[j]);
            }

            vector<Individual> offspring;
            for (size_t i = 0; i < parents.size(); i += 2) {
                Individual child1 = parents[i];
                Individual child2 = parents[i+1];

                double r = (double) rand() / RAND_MAX;
                if (r < crossoverRate && child1.chromosome != child2.chromosome)
                    crossover(child1, child2);

                offspring.push_back(child1);
                offspring.push_back(child2);
            }

            for (auto& child : offspring)
                mutation(child, mutationRate);

            for (auto& child : offspring)
                problem.evaluate(child);

            vector<pair<double, int>> rankOld, rankNew;
            for (size_t i = 0; i < pop.size(); i++)
                rankOld.push_back({pop[i].fitness, (int)i});
            for (size_t i = 0; i < offspring.size(); i++)
                rankNew.push_back({offspring[i].fitness, (int)i});

            sort(rankOld.rbegin(), rankOld.rend());
            sort(rankNew.begin(), rankNew.end());

            for (int k = 0; k < eliteCount && k < (int)pop.size() && k < (int)offspring.size(); k++)
                offspring[rankNew[k].second] = pop[rankOld[k].second];

            pop = offspring;
        }
    }
};
