#include <stdio.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include<errno.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
extern int errno;
void miki(char *arg[],int quees);
char *cadena;
	int tuberia[2];
int main(int argc, char const *argv[]){	
	pipe(tuberia);//la tuberia 
	//crear el primer proceso 

    char *arg[] ={"/bin/ls",NULL};
    miki(arg,1);
    char *arg1[] ={"/usr/bin/cat" ,NULL};
    miki(arg1,3);
    
    char *arg2[] ={"/usr/bin/wc",NULL};
    miki(arg2,2);
	return 0;
}


void miki(char *arg[],int quees){
pid_t pid;
    pid=fork();
    if (pid ==0){
        if (quees==1){
        close(tuberia[0]);
		dup2(tuberia[1],1);
		close(tuberia[1]);//ya no hace falta 
		execv(arg[0],arg);//salida de ls se va a ir a la tuberia

    }
    if (quees==2){
        close(tuberia[1]);
		dup2(tuberia[0],0);
		close(tuberia[0]);//ya no hace falta
		execv(arg[0],arg);//salida de ls se va a ir a la tuberia

    }
    if (quees==3){
        
        dup2(tuberia[1],1);
		close(tuberia[1]);//ya no hace falta 
		dup2(tuberia[0],0);
		close(tuberia[0]);//ya no hace falta
		execl(arg[0],arg[0],NULL);//salida de ls se va a ir a la tuberia

    }
    if(quees>3){
        execv(arg[0],arg);//salida de ls se va a ir a la tuberia
    }
    }
    

	

}