#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(){
    pid_t pid;
    pid = fork();
    int fd = open("archivo.txt", O_WRONLY|O_CREAT|O_TRUNC,0600);
    dup2(fd,1);
    if (pid == 0){
        execl("/usr/bin/ls","usr/bin/ls","-l",NULL);
    }else{
        wait(NULL);
    }
    return 0;
}