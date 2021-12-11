#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <regex>
#include <minisat/core/SolverTypes.h>
#include "minisat/core/Solver.h"

bool bFSearch(std::vector<unsigned> neighbors[], unsigned src, unsigned dest, unsigned nV, unsigned dist[], int prev[]){
// Breadth-first search algorithm referenced from Introduction to Algorithms and Tutorialspoint. Briefly,
//  Push source into queue
//  While queue is not empty, do:
//      get a node (say X) from top of queue,
//      pop queue;
//      visit all neighbors of X,
//      update distance of all neighbors and set their parent to X,
//      if destination is reached, exit algorithm.

    int color[nV]; //0(white), 1(grey), 2(black)
    std::queue<unsigned> q; //initializing queue for BFS
    for (unsigned i=0;i<nV;i++){
        dist[i] = INT_MAX;
        color[i] = 0;//white
        prev[i] = -1;
    }
    dist[src-1] = 0;
    color[src-1] = 1;//grey
    q.push(src);

    //BFS Algorithm:
    while(!q.empty()){
        unsigned x = q.front();
        q.pop();
        for (unsigned i = 0; i < neighbors[x-1].size(); i++) {
            if (color[neighbors[x-1][i]-1] == 0) {
                color[neighbors[x-1][i]-1] = 1;
                dist[neighbors[x-1][i]-1] = dist[x-1] + 1;
                prev[neighbors[x-1][i]-1] = x;
                q.push(neighbors[x-1][i]);

                if (neighbors[x-1][i] == dest)
                   return true;
                color[x-1] = 2;
            }
        }
    }
    return false;
}


int main(void)
{
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    
    unsigned n_vertices = 0;
    unsigned n_edges = 0;
    unsigned source = 0;
    unsigned destination = 0;
    std::vector<unsigned> edge_values;
    
    while (!std::cin.eof()){
        std::string line;
        char command;
        std::getline(std::cin,line);
        command = line[0];
        if (command=='V'){
            if (edge_values.size()>0){
                edge_values.clear();
            }
            std::istringstream input(line);
            input>>n_vertices;
            solver.reset(new Minisat::Solver());
        }
        if (command=='E'){
            bool found_solution = false;
            int temp1, temp2;
            int minimal_k = 0;
            int E_flag = 0;
            std::sregex_iterator iter(line.begin(),line.end(),ex);
            while (iter!=End){
                temp1 = stoi((*iter)[1]);
                temp2 = stoi((*iter)[2]);
                edge_values.push_back(temp1);
                edge_values.push_back(temp2);
                ++iter;
            }
            for (unsigned i=0; i<edge_values.size(); i++){
                if (edge_values[i]>n_vertices){
                    E_flag = 1;
                }
            }
            if (E_flag==1){
                std::cerr<<"Error: vertex specified beyond range.\n";
                continue;
            }
            n_edges = edge_values.size()/2;
            
            
            if (n_edges>0){
                unsigned edges_1[n_edges], edges_2[n_edges];
                for (unsigned i=0; i<n_edges; i++){
                    edges_1[i] = edge_values[2*i];
                    edges_2[i] = edge_values[2*i+1];
                }
            }
            else{
                std::cout<<"\n";
                continue;
            }
            
            for (unsigned k=1; k<=n_vertices; k++){
                
                unsigned n_literals = n_vertices*k;
                unsigned n_clauses[4] = {k,n_edges,int(n_vertices*k*(k-1)/2),int(k*n_vertices*(n_vertices-1)/2),n_edges};
                Minisat::vec<Minisat::Lit> clause_1[n_clauses[0]];
                Minisat::vec<Minisat::Lit> clause_2[n_clauses[1]];
                Minisat::vec<Minisat::Lit> clause_3[n_clauses[2]];
                Minisat::vec<Minisat::Lit> clause_4[n_clauses[3]];
                Minisat::Lit literal_array[n_vertices,k];
                Minisat::Lit* ptr_to_literals = literal_array;
                
                for (unsigned i=0; i<n_vertices; i++){
                    for (unsigned j=0; j<k; j++){
                        literal_array = Minisat::mkLit(solver->newVar());   
                    }
                }
                
                for (unsigned i=0; i<k; i++){
                    for (unsigned j=0; j<n_vertices; j++){
                        clause_1[i].push_back(literal_array[j,i]);
                    }
                }
                
                unsigned count = 0;
                for (unsigned i=0; i<k; i++){
                    for (unsigned j1=0; j1<n_vertices-1; j1++){
                        for (unsigned j2=j1+1; j2<n_vertices; j2++){
                            clause_3[count].push_back(~literal_array[j1,i]);
                            clause_3[count].push_back(~literal_array[j2,i]);
                            count++;
                        }
                    }
                }
                
                count = 0;
                for (unsigned i=0; i<n_vertices; i++){
                    for (unsigned j1=0; j1<k-1; j1++){
                        for (unsigned j2=j1+1; j2<k; j2++){
                            clause_2[count].push_back(~literal_array[i,j1]);
                            clause_2[count].push_back(~literal_array[i,j2]);
                            count++;
                        }
                    }
                }
                
                for (unsigned i=0; i<n_edges; i++){
                    for (unsigned j=0; j<k; j++){
                        clause_4[i].push_back(literal_array[edges_1[i],j]);
                        clause_4[i].push_back(literal_array[edges_2[i],j]);
                    }
                }
                
                for (unsigned i=0; i<4; i++){
                    for (unsigned j=0; j<n_clauses[i]; j++){
                        if (i==0)
                            solver->addClause_(clause_1[j]);
                        if (i==1)
                            solver->addClause_(clause_2[j]);
                        if (i==2)
                            solver->addClause_(clause_3[j]);
                        if (i==3)
                            solver->addClause_(clause_4[j]);
                    }
                }
                clause_1.clear();
                clause_2.clear();
                clause_3.clear();
                clause_4.clear();
                
                minimal_k = k;
                found_solution = solver->solve();
                if (found_solution==true){
                    for (unsigned i=0; i<minimal_k; i++){
                        if (i!=k-1){
                            for (unsigned j=0; j<n_vertices; j++)
                                std::cout<<Minisat::toInt(solver->modelValue(literal_array[j,i]))<<" ";
                        }
                        else {
                            for (unsigned j=0; j<n_vertices-1; j++)
                                std::cout<<Minisat::toInt(solver->modelValue(literal_array[j,i]))<<" ";
                            std::cout<<Minisat::toInt(solver->modelValue(literal_array[n_vertices-1,i]))<<std::endl;
                        }
                    }
                    delete[] ptr_to_literals;
                    break;
                }
                else{
                    delete[] ptr_to_literals;
                    solver.reset(new Minisat::Solver());
                }
            }
        }
        if (command=='s'){
            std::istringstream input_2(line);
            while(!input_2.eof()){
                input_2>>source;
                input_2>>destination;
            }
            if ((source>n_vertices)||(destination>n_vertices)){
                std::cerr<<"Error: specified node(s) not in graph.\n";
            }
            else if ((source<=0)||(destination<=0)){
                std::cerr<<"Error: specified node value(s) out of range.\n";
            }
            else{
                unsigned edges1[n_edges], edges2[n_edges];
                for (unsigned i=0; i<n_edges; i++){
                    edges1[i] = edge_values[2*i];
                    edges2[i] = edge_values[2*i+1];
                }
                std::vector<unsigned> neighborArray[n_vertices];
                for (unsigned i=0; i<n_vertices; i++){
                    for (unsigned j=0; j<n_edges; j++){
                        if ((i+1)==edges1[j]){
                            neighborArray[i].push_back(edges2[j]);
                        }
                        if ((i+1)==edges2[j]){
                            neighborArray[i].push_back(edges1[j]);
                        }
                    }
                }
                unsigned distance[n_vertices];
                int prev[n_vertices];
                if (bFSearch(neighborArray,source,destination,n_vertices,distance,prev)==false){
                    std::cerr<<"Error: No path exists between specified nodes.\n";
                }
                else{
                    std::vector<unsigned> path;
                    unsigned hop = destination;
                    path.push_back(hop);
                    while(prev[hop-1]!=-1){
                        path.push_back(prev[hop-1]);
                        hop = prev[hop-1];
                    }
                    for (unsigned i = path.size() - 1; i >= 1; i--)
                        std::cout << path[i] << "-";
                    std::cout<<path[0]<<std::endl;
                }
            }
        }
    }
    return 0;
}
