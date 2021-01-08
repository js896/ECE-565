#ifndef LANDSCAPE_H
#define LANDSCAPE_H
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h> 

class Landscape{
private:
    int** elevations;
    double** water;
    double** change;
    double** absorbed;
    int N;
    int steps;
    double absorption_rate;
    void new_drop(int i, int j);
    void trickle(int i, int j);
    void absorb(int i, int j);
    bool check_dry();
public:
    int finished_steps;
    Landscape(int steps, double absorption_rate, int N, std::string path);
    void simulate();
    void printResult() const;
};
#endif