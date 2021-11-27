#include <iostream>
#include <unistd.h>

using namespace std;

int main(){
    execlp("python","python","ece650-a1.py",(char*) NULL);
    return 0;
}
