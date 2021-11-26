#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char** argv){
    
    pid_t child_rgen,child_a1,child_a2,child_read;
    int pipes[4];
    pipe(pipes);
    pipe(pipes+2);
    int status;
    
    child_rgen = fork();
    if (child_rgen==0){
        dup2(pipes[1],STDOUT_FILENO);
        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);
        execv("./rgen",argv);
    }
    else{
        child_a1 = fork();
        if (child_a1==0){
            dup2(pipes[0],STDIN_FILENO);
            close(pipes[1]);
            close(pipes[0]);
            dup2(pipes[3],STDOUT_FILENO);
            close(pipes[2]);
            close(pipes[3]);
            exec("./ece650-a1");
        }
        else{
            child_a2 = fork();
            if (child_a2==0){
                dup2(pipes[2],STDIN_FILENO);
                close(pipes[3]);
                close(pipe2[2]);
                close(pipes[1]);
                close(pipes[0]);
                exec("./ece650-a2");
            }
            else{
                child_read = fork();
                if (child_read==0){
                    dup2(pipes[3],STDOUT_FILENO);
                    close(pipes[2]);
                    close(pipes[1]);
                    close(pipes[0]);
                    close(pipes[3]);
                    exec("./readinput");
                }
                else{
                    close(pipes[0]);
                    close(pipes[1]);
                    close(pipes[2]);
                    close(pipes[3]);
                    
                    while (!cin.eof()){
                        wait(&status);
                        kill(-1,SIGTERM);
                    }
                    
                }
            }
        }
    }
    return 0;
}
