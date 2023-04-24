#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
//exec carga el codigo de un programa y lo sobre escribe
int main(){
    pid_t pid;
    printf("hola mundo\n");
    pid = fork();
    if (pid == 0){
        execv("/usr/bin/ls","usr/bin/ls","-l",NULL);
    }else{
        wait(NULL);
    }
    return 0;
}