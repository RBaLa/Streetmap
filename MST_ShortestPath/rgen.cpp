#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <string.h>
#include <random>
#include <unistd.h>
#include <cmath>

using namespace std;

typedef array<signed int,2> two_d_coord;

struct Street{
    string name;
    vector<two_d_coord> segment_endpoints;
};

vector<Street> database;

double distance(two_d_coord a,two_d_coord b){
    double dist;
    int p0_x = a[0];
    int p0_y = a[1];
    int p1_x = b[0];
    int p1_y = b[1];
    dist = sqrt(pow((p0_x-p1_x),2)+pow((p0_y-p1_y),2));
    return dist;
}

bool checkIfPointOnLine(two_d_coord ls_11,two_d_coord ls_12,two_d_coord point){
    bool value = false;
    int p0_x = ls_11[0];
    int p0_y = ls_11[1];
    int p1_x = ls_12[0];
    int p1_y = ls_12[1];
    int p2_x = point[0];
    int p2_y = point[1];
    if ((p2_y-p0_y)*(p1_x-p0_x)==(p1_y-p0_y)*(p2_x-p0_x) &&
        (p1_y-p2_y)*(p1_x-p0_x)==(p1_y-p0_y)*(p1_x-p2_x)){
            double checkdist = distance(ls_11,ls_12);
            if (distance(ls_11,point)<checkdist && 
                distance(ls_12,point)<checkdist)
                    value = true;
        }
    return value;
}

bool checkCollinearity(two_d_coord ls_11,two_d_coord ls_12,two_d_coord ls_21,two_d_coord ls_22){
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
    int det = (-s2_x*s1_y+s1_x*s2_y);
    if (det==0){
        if (checkIfPointOnLine(ls_11,ls_12,ls_21)||checkIfPointOnLine(ls_11,ls_12,ls_22))
            value = true;
        
        if (checkIfPointOnLine(ls_21,ls_22,ls_11)||checkIfPointOnLine(ls_21,ls_22,ls_12))
            value = true;
    }
    return value;
}

bool selfIntersection(two_d_coord ls_11,two_d_coord ls_12,two_d_coord ls_21,two_d_coord ls_22){
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
    int det = (-s2_x*s1_y+s1_x*s2_y);
    if (det!=0){
        double s = double(-s1_y*(p0_x-p2_x)+s1_x*(p0_y-p2_y))/double(det);
        double t = double(s2_x*(p0_y-p2_y)-s2_y*(p0_x-p2_x))/double(det);
        if (s>=0 and s<=1 and t>=0 and t<=1){
            value = true;
        }
    }
    return value;
}

bool suitabilityCheck(two_d_coord a,two_d_coord b,Street st){
    bool value = false;
    if (database.size()==0){
        if (st.segment_endpoints.size()==1){
            if (!(a[0]==b[0]&&a[1]==b[1])){
                value=true;
            }
        }
        else{
            for (unsigned i=0;i<st.segment_endpoints.size()-1;i++){
                if (checkCollinearity(a,b,st.segment_endpoints[i],st.segment_endpoints[i+1])){
                    value = false;
                    break;
                }
                if (st.segment_endpoints.size()>2&&i!=st.segment_endpoints.size()-2
                &&selfIntersection(a,b,st.segment_endpoints[i],st.segment_endpoints[i+1])){
                    value = false;
                    break;
                }
                if (i==st.segment_endpoints.size()-2)
                    value = true;
            }
        }
    }
    else{
        Street temp;
        for (unsigned j=0;j<database.size();j++){
            temp = database[j];
            for (unsigned i=0;i<temp.segment_endpoints.size()-1;i++){
                if (checkCollinearity(a,b,temp.segment_endpoints[i],temp.segment_endpoints[i+1])){
                    value = false;
                    break;
                }
                if (i==temp.segment_endpoints.size()-2)
                    value = true;
            }
            if (value==false)
                break;
            if (j==database.size()-1)
                value = true;
        }
        if (st.segment_endpoints.size()==1){
            if ((a[0]==b[0]&&a[1]==b[1])){
                value=false;
            }
        }
        else{
            for (unsigned i=0;i<st.segment_endpoints.size()-1;i++){
                if (value==false)
                    break;
                if (checkCollinearity(a,b,st.segment_endpoints[i],st.segment_endpoints[i+1])){
                    value = false;
                    break;
                }
                if (st.segment_endpoints.size()>2&&i!=st.segment_endpoints.size()-2
                &&selfIntersection(a,b,st.segment_endpoints[i],st.segment_endpoints[i+1])){
                    value = false;
                    break;
                }
                if (i==st.segment_endpoints.size()-2)
                    value = true;
            }
        }   
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
    uniform_int_distribution<signed int> d_c(-c,c);
    uniform_int_distribution<unsigned> alphabet(0,25);
    uniform_int_distribution<unsigned> name_length(4,8);
    unsigned try_count = 0;
    unsigned segment_try_count = 0;
    unsigned created_streets_count = 0;
    unsigned n_streets,n_segments;
    unsigned sleep_period;
    two_d_coord start,next;
    unsigned name_l;
    char ch;
    string st_name;
    Street st;
    bool flag = true;
    while(!cin.eof()){
        if (database.size()!=0){
            cout<<"rm \"all\""<<endl<<flush;
            database.clear();
        }
        while (true){
            flag = true;
            n_streets = d_s(urandom);
            for (unsigned i=0;i<n_streets;i++){
                name_l = name_length(urandom);
                for (unsigned k=0;k<name_l;k++){
                    ch = 'a'+alphabet(urandom);
                    st.name.push_back(ch);
                }
                st.name.append(" street");
                start[0] = d_c(urandom);
                start[1] = d_c(urandom);
                st.segment_endpoints.push_back(start);
                n_segments = d_n(urandom);
                for (unsigned j=0;j<n_segments;j++){
                    next[0] = d_c(urandom);
                    next[1] = d_c(urandom);
                    while (!suitabilityCheck(start,next,st)){
                        segment_try_count++;
                        next[0] = d_c(urandom);
                        next[1] = d_c(urandom);
                        if (segment_try_count==25){
                            flag = false;
                            break;
                        }
                    }
                    if (flag==false)
                        break;
                    segment_try_count = 0;
                    st.segment_endpoints.push_back(next);
                    start[0] = next[0];
                    start[1] = next[1];
                }
                if (flag==false){
                    try_count++;
                    database.clear();
                    if (try_count==25){
                        cerr<<"Eror: Rgen failed to generate street for 25 continous attempts.\n"<<flush;
                        exit(1);
                    }
                    break;
                }
                created_streets_count++;
                database.push_back(st);
                st.name.clear();
                st.segment_endpoints.clear();
            }
            if (created_streets_count==n_streets){
                created_streets_count = 0;
                break;
            }
        }

        for (unsigned i=0;i<database.size();i++){
            cout<<"add \""<<database[i].name<<"\" ";
            for (unsigned j=0;j<database[i].segment_endpoints.size();j++){
                cout<<"("<<database[i].segment_endpoints[j][0]<<","<<database[i].segment_endpoints[j][1]<<") ";
            }
            cout<<endl<<flush;
        }
        cout<<"gg\n"<<flush;
        sleep_period = d_l(urandom);
        sleep(sleep_period);
    }
    return 0;
}
