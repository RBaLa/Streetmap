#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

using namespace std;

int readinput(int& pipe2_read,int& pipe2_write){
    dup2(pipe2_write,STDOUT_FILENO);
    close(pipe2_read);
    close(pipe2_write);
    string line;
    while (!cin.eof()){
        getline(cin,line);
        if (line.size()>1)
            cout<<line<<endl;
        if (cin.eof())
            break;
    }
    return 0;
}

int main(int argc, char** argv){
    
    pid_t child_rgen,child_a1,child_a2;
    int pipe1[2],pipe2[2];
    int status;
    pipe(pipe1);
    pipe(pipe2);
    
    child_rgen = fork();
    if (child_rgen==0){
        dup2(pipe1[WRITE_END],STDOUT_FILENO);
        close(pipe1[READ_END]);
        close(pipe1[WRITE_END]);
        close(pipe2[READ_END]);
        close(pipe2[WRITE_END]);
        execv("./rgen",argv);
    }
    else{
        child_a1 = fork();
        if (child_a1==0){
            dup2(pipe1[READ_END],STDIN_FILENO);
            close(pipe1[WRITE_END]);
            close(pipe1[READ_END]);
            dup2(pipe2[WRITE_END],STDOUT_FILENO);
            close(pipe2[READ_END]);
            close(pipe2[WRITE_END]);
            execlp("python","python","ece650-a1.py",(char*)NULL);
        }
        else{
            child_a2 = fork();
            if (child_a2==0){
                dup2(pipe2[READ_END],STDIN_FILENO);
                close(pipe2[WRITE_END]);
                close(pipe2[READ_END]);
                close(pipe1[WRITE_END]);
                close(pipe1[READ_END]);
                execlp("./ece650-a2",NULL);
            }
            else{
                int child_read = fork();
                if (child_read==0){
                    return readinput(pipe2[READ_END],pipe2[WRITE_END]);
                }
                else{
                    close(pipe2[WRITE_END]);
                    close(pipe2[READ_END]);
                    close(pipe1[WRITE_END]);
                    close(pipe1[READ_END]);
                    waitpid(child_read,&status,0);
                    kill(child_a1,SIGTERM);
                    kill(child_a2,SIGTERM);
                    kill(child_rgen,SIGTERM);
                }
            }
        }
    }
    return 0;
}
