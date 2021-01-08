#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

#define N 1024

double A[N][N];
double B[N][N];
double C[N][N];
map<string, int> ordering_map;

void i_j_k() {
    double sum;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void j_k_i() {
    double tmp;
    for (int j = 0; j < N; ++j) {
        for (int k = 0; k < N; ++k) {
            tmp = B[k][j];
            for (int i =0; i < N; ++i) {
                C[i][j] += tmp * A [i][k];
            }
        }
    }
}

void i_k_j() {
    double tmp;
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            tmp = A[i][k];
            for (int j = 0; j < N; ++j) {
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void loop_tilling() {
    double sum;
    for (int i = 0; i < N; i += 1024) { 
        for (int j = 0; j < N; j += 1024) { 
            for (int ii = i; ii < (i + 1024); ++ii){
                for (int jj = j; jj < (j + 1024); ++jj){
                     sum = 0; 
                        for (int k = 0; k < N; ++k) {
                            sum += A[ii][k] * B[k][jj]; 
                        }
                    C[ii][jj] = sum;
                }
            }
        }
    }
}

void init_matrices() {
    for (int row = 0; row < N; ++row) {
        for (int colomn = 0; colomn < N; ++colomn) {
            A[row][colomn] = row + colomn;
            B[row][colomn] = row + colomn + rand();
        }
    }
}

void init_map() {
    ordering_map.insert(pair<string, int>("i-j-k", 0));
    ordering_map.insert(pair<string, int>("j-k-i", 1)); 
    ordering_map.insert(pair<string, int>("i-k-j", 2));
    ordering_map.insert(pair<string, int>("loop-tilling", 3));
}

int main(int argc, char **argv)
{
 
    init_map();
    init_matrices();
    clock_t tStart;
    srand(time(NULL));
    switch (ordering_map[argv[1]])
    {
    case 0:
        tStart = clock();
        i_j_k();
        cout << "Ordering: i-j-k, time = " << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl; 
        break;
    case 1:
        tStart = clock();
        j_k_i();
        cout << "Ordering: j-k-i, time = " << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
        break;
    case 2:
        tStart = clock();
        i_k_j();
        cout << "Ordering: i-k-j, time = " << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
        break;
    default :
        tStart = clock();
        loop_tilling();
        cout << "Loop tilling, time = " << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
        break;
    }


    return EXIT_SUCCESS;
}
