#pragma once
#include "../Problem.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Clause {
    vector<int> literals;
};

class ThreeSATProblem : public Problem {

private:
    vector<Clause> clauses;
    void loadFile(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            if (line.empty())
                continue;

            if (line[0] == 'c' || line[0] == 'p')
                continue;
            
            stringstream ss(line);

            Clause clause;

            int literal;

            while (ss >> literal) {
                if (literal == 0)
                    break;
                clause.literals.push_back(literal);
            }
            if (!clause.literals.empty())
                clauses.push_back(clause);        
        }
    }

    bool evaluateClause(const Clause& clause, const vector<int>& chromosome) {
        for (int literal : clause.literals) {

            int varIndex = abs(literal) - 1;

            bool value = chromosome[varIndex];

            if (literal > 0 && value)
                return true;

            if (literal < 0 && !value)
                return true;
        }

        return false;
    }

public:
    ThreeSATProblem(const string& filename) {
        loadFile(filename);
    }

    void evaluate(Individual& ind) override {
        int satisfied = 0;

        for (const Clause& clause : clauses) {
            if (evaluateClause(clause, ind.chromosome))
                satisfied++;
        }

        ind.fitness = satisfied;

        ind.properties["sat"] = satisfied;
        ind.properties["total"] = clauses.size();
    }
};