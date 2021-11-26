#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <climits>
#include <queue>
#include <random>

using namespace std;

int main(int argc, char** argv){
    
    random_device urandom("/dev/urandom");
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;
    
    for (int i=0;i<argc;++i){
        if (strcmp(argv[i], "-s") == 0){
            s = stoi(argv[i+1]);
        }
        if (strcmp(argv[i], "-n") == 0){
            n = stoi(argv[i+1]);
        }
        if (strcmp(argv[i], "-l") == 0){
            l = stoi(argv[i+1]);
        }
        if (strcmp(argv[i], "-c") == 0){
            c = stoi(argv[i+1]);
        }
    }
    uniform_int_distribution<unsigned> d_s(2,s);
    uniform_int_distribution<unsigned> d_n(1,n);
    uniform_int_distribution<unsigned> d_l(5,l);
    uniform_int_distribution<unsigned> d_c(-c,c);
    
    
    
}
