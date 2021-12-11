#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <minisat/core/SolverTypes.h>
#include "minisat/core/Solver.h"

int main()
{
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    
    unsigned n_vertices = 0;
    unsigned n_edges = 0;
    vector<unsigned> edge_values;
    
    while (!cin.eof()){
        string line;
        char command;
        getline(cin,line);
        command = line[0];
        if (command=='V'){
            if (edgeValues.size()>0){
                edgeValues.clear();
            }
            istringstream input(line);
            input>>n_vertices;
            solver.reset (new Minisat::Solver());
        }
        if (command=='E'){
            int temp1, temp2;
            int E_flag = 0;
            sregex_iterator iter(line.begin(),line.end(),ex);
            while (iter!=End){
                temp1 = stoi((*iter)[1]);
                temp2 = stoi((*iter)[2]);
                edge_values.push_back(temp1);
                edge_values.push_back(temp2);
                ++iter;
            }
            for (unsigned i=0; i<edgeValues.size(); i++){
                if (edge_values[i]>n_vertices){
                    E_flag = 1;
                }
            }
            if (E_flag==1){
                cerr<<"Error: vertex specified beyond range.\n";
                continue;
            }
            n_edges = edge_values.size()/2;
        
            
            
            
        }
    }
}
