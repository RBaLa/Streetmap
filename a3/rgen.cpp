#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <string.h>
#include <random>
#include <unistd.h>

using namespace std;

class database{
    public:
        list<string> street_names;
        list<string>::iterator street_id;
        
};

class street{
    public:
        typedef vector<int> arr;
        list<arr> segment_endpoints;
        list<arr>::iterator se_id;
};

bool checkCollinearity(vector<int> ls_11,vector<int> ls_12,vector<int> ls_21,vector<int> ls_22){
    bool value = false;
    int p0_x = ls_11[0];
    int p0_y = ls_11[1];
    int p1_x = ls_12[0];
    int p1_y = ls_12[1];
    int p2_x = ls_21[0];
    int p2_y = ls_21[1];
    int p3_x = ls_22[0];
    int p3_y = ls_22[1];

    int s1_x = p1_x-p0_x;
    int s2_x = p3_x-p2_x;
    int s1_y = p1_y-p0_y;
    int s2_y = p3_y-p2_y;
    float det = (-s2_x*s1_y+s1_x*s2_y);
    if (det==0){
        //check which of the two segments are smaller
        //check if any of the two points of the smaller are on the larger
        //value = true if above condition is fulfilled
    }

    return value;
}

int main(int argc, char** argv){
    
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
    random_device urandom("/dev/urandom");
    uniform_int_distribution<unsigned> d_s(2,s);
    uniform_int_distribution<unsigned> d_n(1,n);
    uniform_int_distribution<unsigned> d_l(5,l);
    uniform_int_distribution<unsigned> d_c(-c,c);
    
    unsigned try_count = 0;
    unsigned second = 1000000;
    while(!cin.eof()){

        //for s streets:
        //   get a n value
        unsigned n_streets = d_n(urandom);
        //   choose a starting point in [-l,l]x[-l,l] which is not part of any street in database
        
        //   if no such point exists, exit(1)
        //   for n segments:
        //      chose next point such that formed segment is not collinear with any existing segment
        //      if no such point exists, break out of this loop and decrement i; try_count++; if try_...
        //      set chosen point as starting point for next segment
        //if fail->
        try_count++;
        if (try_count>=25){
            exit(1);
        }
        unsigned sleep_period = d_l(urandom);
        usleep(sleep_period*second);
    }
    return 0;
}
