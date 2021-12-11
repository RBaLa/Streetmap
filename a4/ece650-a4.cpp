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
    unsigned source = 0;
    unsigned destination = 0;
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
            bool found_solution = false;
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
            
            Minisat::vec<Minisat::Lit> clause_4[n_edges];
            
            for (unsigned k=1; k<=n_vertices; k++){
                
                unisigned n_literals = n_vertices*k;
                Minisat::vec<Minisat::Lit> clause_1[k];
                Minisat::vec<Minisat::Lit> clause_2[int(n_vertices*k*(k-1)/2)];
                Minisat::vec<Minisat::Lit> clause_3[int(k*n_vertices*(n_vertices-1)/2)];
                Minisat::Lit literal_array[n_vertices,k];
                
                for (unsigned i=0; i<n_vertices; i++){
                    for (unsigned j=0; j<k; j++){
                        literal_array = Minisat::mkLit(solver->newVar());   
                    }
                }
                
                
                if (found_solution==true){
                    break;
                }
            }
            
            
            
        }
        if (command=='s'){
            istringstream input_2(line);
            while(!input_2.eof()){
                input_2>>source;
                input_2>>destination;
            }
            if ((source>nVertices)||(destination>nVertices)){
                cerr<<"Error: specified node(s) not in graph.\n";
            }
            else if ((source<=0)||(destination<=0)){
                cerr<<"Error: specified node value(s) out of range.\n";
            }
            else{
                unsigned edges1[nEdges], edges2[nEdges];
                for (unsigned i=0; i<nEdges; i++){
                    edges1[i] = edgeValues[2*i];
                    edges2[i] = edgeValues[2*i+1];
                }
                vector<unsigned> neighborArray[nVertices];
                for (unsigned i=0; i<nVertices; i++){
                    for (unsigned j=0; j<nEdges; j++){
                        if ((i+1)==edges1[j]){
                            neighborArray[i].push_back(edges2[j]);
                        }
                        if ((i+1)==edges2[j]){
                            neighborArray[i].push_back(edges1[j]);
                        }
                    }
                }
                unsigned distance[nVertices];
                int prev[nVertices];
                if (bFSearch(neighborArray,source,destination,nVertices,distance,prev)==false){
                    cerr<<"Error: No path exists between specified nodes.\n";
                }
                else{
                    vector<unsigned> path;
                    unsigned hop = destination;
                    path.push_back(hop);
                    while(prev[hop-1]!=-1){
                        path.push_back(prev[hop-1]);
                        hop = prev[hop-1];
                    }
                    for (unsigned i = path.size() - 1; i >= 1; i--)
                        cout << path[i] << "-";
                    cout<<path[0]<<endl;
                }
            }

    }
}
