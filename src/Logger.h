#pragma once
#include <fstream>
#include <string>
using namespace std;

class Logger {
private:
    ofstream file;

public:
    Logger(const string& filename) {
        file.open(filename);
        file << "generation,best,average,best_value,average_value\n";
    }

    void log(int gen, double best, double avg, double bestValue, double avgValue) {
        file << gen << "," << best << "," << avg << "," << bestValue << "," << avgValue << "\n";
    }

    ~Logger() {
        file.close();
    }
};