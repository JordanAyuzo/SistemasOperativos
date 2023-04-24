#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include<errno.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int tuberia[2];
void childmake(char* path,char** args,int in,int out, char* nombreComando){  
    pid_t pid = fork(); 
    
    if (pid==0){
        //poner en la tuberia
        if(in == 1){
            close(tuberia[1]);
		    dup2(tuberia[0],0);
		    close(tuberia[0]);//ya no hace falta
            execv(path,args);
        }
        if (out == 1){
            close(tuberia[0]);
		    dup2(tuberia[1],1);
		    close(tuberia[1]);//ya no hace falta 
            execv(path,args);
        }    

        //ejecutar el comando
        execv(path,args);
        printf("\033[31m Comando \033[33m\"%s\"\033[31m no reconocido.Intenta con otro comando\033[0m\n",nombreComando);
        exit(-1);
    }else{
        //esperamos a que termine el proceso hijo
         wait(NULL);
    } 
}

int main(){
    pipe(tuberia);//la tuberia 
    char *args[] = { "/bin/ls", "-l", NULL };
       close(tuberia[0]);
    childmake(args[0],args,0,1,"ls");
    close(tuberia[1]);

    char *args2[] ={"/usr/bin/wc",NULL};
    childmake(args2[0],args2,0,1,"ls");
    close(tuberia[0]);
    close(tuberia[1]);
}

