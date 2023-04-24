#include <stdio.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include<errno.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
extern int errno;
int y;
int main(int argc, char const *argv[]){	
	//proceso padre
	char *cadena;
	int tuberia[2];
	pid_t pid;
    int hola;
	pipe(tuberia);//la tuberia 
	//crear el primer proceso 
	pid = fork();  
	//proceso hijo 1 
	if (pid==0)
	{	
		close(tuberia[0]);
		dup2(tuberia[1],1);
		close(tuberia[1]);//ya no hace falta 
		execl("/bin/ls","/bin/ls","-l",NULL);//salida de ls se va a ir a la tuberia 
		printf("hola mundo"); el printf se ejecuta solo si el execl no se ejecuto de manera adecuada 
	}
	pid=fork();
	//proceso hijo2 
	if (pid==0)
	{
		close(tuberia[1]);
		dup2(tuberia[0],0);
		close(tuberia[0]);//ya no hace falta
		execl("/usr/bin/wc","/usr/bin/wc",NULL);//salida de ls se va a ir a la tuberia 

	}
	return 0;
}