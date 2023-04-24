//https://man7.org/linux/man-pages/man7/signal.7.html

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int kill(pid_t pid, int sig);
int main(){
    int signal;
    while (1)
    {
        scanf("%d",&signal);
        printf("%d",kill(1184,signal));
    }
}

//tuberias
//fork 
// pipe