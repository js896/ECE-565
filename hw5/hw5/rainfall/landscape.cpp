#include "landscape.h"

Landscape:: Landscape(int steps, double absorption_rate, int N, std::string path): steps(steps),  absorption_rate(absorption_rate), N(N){
    this->finished_steps = steps;
    this->water = new double * [N];    
    this->absorbed = new double* [N];
    this->change = new double*[N];
    for(int i = 0; i < N; i++){
        this->change[i] = new double[N];
        this->absorbed[i] = new double[N];
        this->water[i] = new double[N];
        for (int j = 0; j < N; ++j) {
            this->water[i][j] = 0;
            this->absorbed[i][j] = 0;
            this->change[i][j] = 0;
        }
    }
    this->elevations = new int* [N];
    for(int i = 0; i < N; i++){
       this->elevations[i] = new int[N];
    }
    std::vector<std::vector<int>> array;
    std::fstream input_file;
    input_file.open(path, std::ios::in);
    if (input_file.is_open()){
      std::string line;
      
      while(getline(input_file, line)){
      std::vector<int> int_in_current_line;
        std::istringstream ss(line);
        std::string out_temp;
         while (ss >> out_temp) {
        // print the read word
        int_in_current_line.push_back(std::stoi(out_temp));
        }
        array.push_back(int_in_current_line);
      }
      for (int i = 0; i < N; i++){
          for (int j =0; j < N; j++){
              this->elevations[i][j] = array[i][j];
          }
      }
      input_file.close(); //close the file object.
   }
}

void Landscape:: new_drop(int i, int j) {
    ++water[i][j];
}

void Landscape:: absorb(int i, int j) {
    double absorb_amount = (absorption_rate <= water[i][j])? absorption_rate: water[i][j];
    absorbed[i][j] += absorb_amount;
    water[i][j] -= absorb_amount;
}

void Landscape:: trickle(int i, int j) {
    std::vector<int>height;
    int direction[] = {0, 0, 0, 0 };
    height.push_back(elevations[i][j]);
    if(i > 0){
       height.push_back(elevations[i-1][j]);
    } 
    if (i + 1 < N) {
        height.push_back(elevations[i+1][j]);
    }
    if (j > 0) {
        height.push_back(elevations[i][j-1]);
    }
    if (j + 1 < N) {
       height.push_back(elevations[i][j+1]);
    }
    double lowest = *min_element(height.begin(), height.end()); 
    if (elevations[i][j] == lowest){
        return;
    }
    //north
    if (i > 0 && elevations[i - 1][j] == lowest){
        direction[0] = 1;
    }
    //south
    if (i + 1  < N && elevations[i+1][j] == lowest){
        direction[1] = 1;
    }
    //west
    if (j > 0 && elevations[i][j-1] == lowest){
        direction[2] = 1;
    }
    //east
     if (j + 1 < N && elevations[i][j+1] == lowest){
        direction[3] = 1;
    }
    int cnt = 0;
    for (int i = 0; i < 4; ++i){
        if (direction[i] == 1) {
            ++cnt;
        }
    }
    //int change[N][N] = {0};
    double amt = (water[i][j] >= 1) ? 1 : water[i][j];
    
    for (int k = 0; k < 4; k++){
        if (direction[k] == 1){
            switch (k)
            {
            //north
            case 0:
                change[i - 1][j] += amt/cnt;
                break;
            //south
            case 1:
                change[i + 1][j] += amt/cnt;
                break;
            //west
            case 2:
                change[i][j-1] += amt/cnt;
                break;
            //east
            default:
                change[i][j+1] += amt/cnt;
                break;
            }
        }
    }
    water[i][j] -= amt;
} 

void Landscape::printResult() const {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << absorbed[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool Landscape::check_dry() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (this->water[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}

void Landscape::simulate(){
    for(int k = 0; k < this->steps; k++){
        for(int ii = 0; ii < N; ii++){
            for(int jj = 0; jj < N; jj++){
                change[ii][jj] = 0;
            }
        }
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                this->new_drop(i, j);
                this->absorb(i, j);
                this->trickle(i, j);
            }
        }

        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                this->water[i][j] += this->change[i][j];
            }
        }
    }
    while (!this->check_dry()) {
        for(int ii = 0; ii < N; ii++){
            for(int jj = 0; jj < N; jj++){
                change[ii][jj] = 0;
            }
        }
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                this->absorb(i, j);
                this->trickle(i, j);
            }
        }

        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                this->water[i][j] += this->change[i][j];
            }
        }
        ++finished_steps;
    }
    return;
}