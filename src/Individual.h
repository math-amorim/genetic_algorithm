#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct Individual {
    vector<int> chromosome;
    double fitness = 0.0;
    std::unordered_map<std::string, double> properties;
};