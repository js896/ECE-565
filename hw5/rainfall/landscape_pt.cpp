#include "landscape_pt.h"
//rule of three

Landscape::Landscape(const Landscape& other):
    N(other.N),
    steps(other.steps),
    num_threads(other.num_threads),
    absorption_rate(other.absorption_rate),
    finished_steps(other.finished_steps)
{
    this->finished_steps = steps;
    this->water = new double * [N];    
    this->absorbed = new double* [N];
    this->change = new double*[N];
    this->elevations = new int*[N];
    for(int i = 0; i < N; i++){
        this->change[i] = new double[N];
        this->absorbed[i] = new double[N];
        this->water[i] = new double[N];
        this->elevations[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            this->water[i][j] = other.water[i][j];
            this->absorbed[i][j] = other.absorbed[i][j];
            this->change[i][j] = other.change[i][j];
            this->elevations[i][j] = other.elevations[i][j];
        }
    }
    this->locks = new pthread_mutex_t[N];
    for (int i = 0; i < N; ++i) {
        this->locks[i] = other.locks[i];
    }
}

Landscape& Landscape::operator=(const Landscape& other){
    if(&other == this){
        return * this;
    }
    this->N = other.N;
    this->steps = other.steps;
    this->num_threads = other.num_threads;
    this->absorption_rate = other.absorption_rate;
    this->finished_steps = other.finished_steps;
    this->finished_steps = steps;
    this->water = new double * [N];    
    this->absorbed = new double* [N];
    this->change = new double*[N];
    this->elevations = new int*[N];
    for(int i = 0; i < N; i++){
        this->change[i] = new double[N];
        this->absorbed[i] = new double[N];
        this->water[i] = new double[N];
        this->elevations[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            this->water[i][j] = other.water[i][j];
            this->absorbed[i][j] = other.absorbed[i][j];
            this->change[i][j] = other.change[i][j];
            this->elevations[i][j] = other.elevations[i][j];
        }
    }
    this->locks = new pthread_mutex_t[N];
    for (int i = 0; i < N; ++i) {
        this->locks[i] = other.locks[i];
    }
    return *this;
}

Landscape::~Landscape(){
    delete[]locks;
    for(int i = 0; i < N; i++){
        delete [] absorbed[i];
        delete [] change[i];
        delete [] water[i];
        delete [] elevations[i];
    }
    delete[]absorbed;
    delete[]change;
    delete[]water;
    delete[]elevations;
}


Landscape:: Landscape(int num_threads, int steps, double absorption_rate, int N, std::string path): num_threads(num_threads), steps(steps),  absorption_rate(absorption_rate), N(N){
    this->locks = (pthread_mutex_t*)malloc(N * sizeof(pthread_mutex_t));
    for (int i = 0; i < N; ++i) {
        pthread_mutex_init(&locks[i], NULL);
    }
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
                pthread_mutex_lock(&this->locks[i - 1]);
                change[i - 1][j] += amt/cnt;
                pthread_mutex_unlock(&this->locks[i - 1]);
                break;
            //south
            case 1:
                pthread_mutex_lock(&this->locks[i + 1]);
                change[i + 1][j] += amt/cnt;
                pthread_mutex_unlock(&this->locks[i + 1]);
                break;
            //west
            case 2:
                pthread_mutex_lock(&this->locks[i]);
                change[i][j-1] += amt/cnt;
                pthread_mutex_unlock(&this->locks[i]);
                break;
            //east
            default:
                pthread_mutex_lock(&this->locks[i]);
                change[i][j+1] += amt/cnt;
                pthread_mutex_unlock(&this->locks[i]);
                break;
            }
        }
    }
    water[i][j] -= amt;
} 

void Landscape::printResult() const {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << std::right << std::setw(8) << absorbed[i][j] << " ";
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




void Landscape::rain(int threadId){
    int low_bound = threadId * (this->N / this->num_threads);
    int up_bound = (threadId + 1) * (this->N / this->num_threads);
    //std::cout << "low_bround" << low_bound << std::endl;
    //std::cout << "up_bound" << up_bound <<std::endl;
    
        for(int i = low_bound; i < up_bound; i++){
            for(int j = 0; j < N; j++){
                this->new_drop(i, j);
                this->absorb(i, j);
                this->trickle(i, j);
            }
        }
        /*
        for (int i = low_bound; i < up_bound; i++){
            for (int j = 0; j < N; j++){
                this->water[i][j] += this->change[i][j];
            }
        }
        */
    
}

void Landscape::drain (int threadId) {
    int low_bound = threadId * (this->N / this->num_threads);
    int up_bound = (threadId + 1) * (this->N / this->num_threads);
    //std::cout << "low_bround" << low_bound << std::endl;
    //std::cout << "up_bound" << up_bound <<std::endl;
        
        for(int i = low_bound; i < up_bound; i++){
            for(int j = 0; j < N; j++){
                this->absorb(i, j);
                this->trickle(i, j);
            }
        }
        /*
        for (int i = low_bound; i < up_bound; i++){
            for (int j = 0; j < N; j++){
                this->water[i][j] += this->change[i][j];
            }
        }
    */
}
void Landscape::reset_change(int threadId){
    int low_bound = threadId * (this->N / this->num_threads);
    int up_bound = (threadId + 1) * (this->N / this->num_threads);
    for (int i = low_bound; i < up_bound; i++){
        for (int j = 0; j < N; j++){
            this->change[i][j] = 0;
        }
    }
}
void Landscape::water_change(int threadId) {
    int low_bound = threadId * (this->N / this->num_threads);
    int up_bound = (threadId + 1) * (this->N / this->num_threads);
    for (int i = low_bound; i < up_bound; i++){
        for (int j = 0; j < N; j++){
            this->water[i][j] += this->change[i][j];
        }
    }
}

void Landscape::add_one_step(){
    //std::cout << "sdfadfaf";
    ++this->finished_steps;
}