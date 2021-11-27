#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <string.h>

using namespace std;

int main(){
    while (!cin.eof()){
        string line;
        getline(cin, line);
        cout<<line<<endl;
    }
    return 0;
}
