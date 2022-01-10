#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <regex>
#include <string.h>

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
    
    unsigned s,n,l,c;

    for (int i=0;i<argc;++i){
        if (strcmp(argv[i], "-s") == 0){
            s = stoi(argv[i+1]);
            if (s<2){
                cerr<<"Error: -s value in command line arg < 2"<<endl;
                exit(1);
            }
        }
        if (strcmp(argv[i], "-n") == 0){
            n = stoi(argv[i+1]);
            if (n<1){
                cerr<<"Error: -n value in command line arg < 1"<<endl;
                exit(1);
            }
        }
        if (strcmp(argv[i], "-l") == 0){
            l = stoi(argv[i+1]);
            if (l<5){
                cerr<<"Error: -l value in command line arg < 5"<<endl;
                exit(1);
            }
        }
        if (strcmp(argv[i], "-c") == 0){
            c = stoi(argv[i+1]);
            if (c<1){
                cerr<<"Error: -c value in command line arg < 1"<<endl;
                exit(1);
            }
        }
    }

    pid_t child_rgen,child_gg,child_sp;
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
        child_gg = fork();
        if (child_gg==0){
            dup2(pipe1[READ_END],STDIN_FILENO);
            close(pipe1[WRITE_END]);
            close(pipe1[READ_END]);
            dup2(pipe2[WRITE_END],STDOUT_FILENO);
            close(pipe2[READ_END]);
            close(pipe2[WRITE_END]);
            execlp("python","python","getgraph.py",(char*)NULL);
        }
        else{
            child_sp = fork();
            if (child_sp==0){
                dup2(pipe2[READ_END],STDIN_FILENO);
                close(pipe2[WRITE_END]);
                close(pipe2[READ_END]);
                close(pipe1[WRITE_END]);
                close(pipe1[READ_END]);
                execlp("./sp",NULL);
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
                    kill(child_gg,SIGTERM);
                    kill(child_sp,SIGTERM);
                    kill(child_rgen,SIGTERM);
                }
            }
        }
    }
    return 0;
}
