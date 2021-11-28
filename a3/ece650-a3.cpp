#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

using namespace std;

int main(int argc, char** argv){
    
    pid_t child_rgen,child_a1,child_a2;
    int pipe1[2],pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    
    child_rgen = fork();
    if (child_rgen==0){
        int dup2check1 = dup2(pipe1[WRITE_END],STDOUT_FILENO);
        close(pipe1[READ_END]);
        close(pipe1[WRITE_END]);
        close(pipe2[READ_END]);
        close(pipe2[WRITE_END]);
        execv("./rgen",argv);
    }
    else{
        child_a1 = fork();
        if (child_a1==0){
            int dup2check2 = dup2(pipe1[READ_END],STDIN_FILENO);
            close(pipe1[WRITE_END]);
            close(pipe1[READ_END]);
            int dup2check3 = dup2(pipe2[WRITE_END],STDOUT_FILENO);
            close(pipe2[READ_END]);
            close(pipe2[WRITE_END]);
            execlp("python","python","ece650-a1.py",(char*)NULL);
        }
        else{
            child_a2 = fork();
            if (child_a2==0){
                int dup2check4 = dup2(pipe2[READ_END],STDIN_FILENO);
                close(pipe2[WRITE_END]);
                close(pipe2[READ_END]);
                close(pipe1[WRITE_END]);
                close(pipe1[READ_END]);
                execlp("./ece650-a2",(char*)NULL);
            }
            else{
                //dup2(pipe2[WRITE_END],STDOUT_FILENO);
                close(pipe1[READ_END]);
                close(pipe1[WRITE_END]);
                close(pipe2[READ_END]);
                close(pipe2[WRITE_END]);
                string line;
                while (!cin.eof()){
                    //getline(cin,line);
                    //cout<<line<<endl;
                    if (cin.eof())
                        break;
                }
                kill(child_a1,SIGTERM);
                kill(child_a2,SIGTERM);
                kill(child_rgen,SIGTERM);
            }
        }
    }
    return 0;
}
