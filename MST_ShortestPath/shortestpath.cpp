#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <climits>
#include <queue>

using namespace std;

bool bFSearch(vector<unsigned> neighbors[], unsigned src, unsigned dest, unsigned nV, unsigned dist[], int prev[]){
// Breadth-first search algorithm referenced from Introduction to Algorithms and Tutorialspoint. Briefly,
//  Push source into queue
//  While queue is not empty, do:
//      get a node (say X) from top of queue,
//      pop queue;
//      visit all neighbors of X,
//      update distance of all neighbors and set their parent to X,
//      if destination is reached, exit algorithm.

    int color[nV]; //0(white), 1(grey), 2(black)
    queue<unsigned> q; //initializing queue for BFS
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

int main() {
    regex ex("\\<(\\d+),(\\d+)\\>");
    sregex_iterator End;
    unsigned nVertices = 0;
    unsigned nEdges = 0;
    unsigned source = 0;
    unsigned destination = 0;
    vector<unsigned> edgeValues;
    string line;
    while (!cin.eof()) {
        char command;
        cin>>command;
        if (command=='\n'){
            cin.ignore();
            continue;
        }
        else{
            getline(cin,line);
        }
        if (command=='V'){
            if (edgeValues.size()>0){
                edgeValues.clear();
            }
            istringstream vinput(line);
            vinput>>nVertices;
            cout<<"V "<<nVertices<<endl;
        }
        if (command=='E'){
            int temp1, temp2;
            int E_flag = 0;
            sregex_iterator iter(line.begin(),line.end(),ex);
            while (iter!=End){
                temp1 = stoi((*iter)[1]);
                temp2 = stoi((*iter)[2]);
                edgeValues.push_back(temp1);
                edgeValues.push_back(temp2);
                ++iter;
            }
            for (unsigned i=0; i<edgeValues.size(); i++){
                if (edgeValues[i]>nVertices){
                    E_flag = 1;
                }
            }
            if (E_flag==1){
                cerr<<"Error: vertex specified beyond range.\n";
                continue;
            }
            nEdges = edgeValues.size()/2;
            
            if (nEdges>1){
                cout<<"E {";
                for (unsigned i=0;i<nEdges-1;i++){
                    cout<<"<"<<edgeValues[2*i]<<","<<edgeValues[2*i+1]<<">,";
                }
                cout<<"<"<<edgeValues[2*(nEdges-1)]<<","<<edgeValues[2*(nEdges-1)+1]<<">}\n";
            }
            else if (nEdges==1){
                cout<<"E {<"<<edgeValues[0]<<","<<edgeValues[1]<<">}\n";
            }
            else{
                cout<<"E {}\n";
            }
        }
        if (command=='s'){
            istringstream input(line);
            while(!input.eof()){
                input>>source;
                input>>destination;
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
    return(0);
}

