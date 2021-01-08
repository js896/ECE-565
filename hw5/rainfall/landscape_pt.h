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
#include <thread>
#include <chrono>
#include <utility>
#include <iomanip>

class Landscape{
private:
    int** elevations;
    double** water;
    double** change;
    double** absorbed;
    int N;
    int num_threads;
    double absorption_rate;
    void new_drop(int i, int j);
    void trickle(int i, int j);
    void absorb(int i, int j);
    pthread_mutex_t* locks;
public:
    int steps;
    int finished_steps;
    Landscape(int num_threads, int steps, double absorption_rate, int N, std::string path);
    Landscape(const Landscape& other);
    Landscape& operator=(const Landscape& other);
    ~Landscape();
    void rain(int threadId);
    void printResult() const;
    bool check_dry();
    void drain(int threadId);
    void add_one_step();
    void water_change(int threadId);
    void reset_change(int threadId);
};
#endif