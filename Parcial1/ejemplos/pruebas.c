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
#define MAX_PATH_LENGTH 1024
int tuberia[2];
void childmake(int probandoFuncion,int out,int in, char* nombreComando){  
    pid_t pid = fork(); 
    if (pid==0){
        //poner en la tuberia
        if (out==1){
            close(tuberia[0]);
		    dup2(tuberia[1],1);
		    close(tuberia[1]); 
        }
        //obtener de la tuberia
        if (in==1){
            close(tuberia[1]);
		    dup2(tuberia[0],0);
		    close(tuberia[0]);
        }
        //ejecutar el comando
        if(probandoFuncion == 1){
            char *args[] = { "/bin/ls", "-l", NULL };
            execv(args[0], args);
        }else{
            char *args[] = { "/usr/bin/wc",NULL };
            execv(args[0], args);
        }

        printf("\033[31m Comando \033[33m\"%s\"\033[31m no reconocido.Intenta con otro comando\033[0m\n",nombreComando);
        exit(-1);
    }else{
        //esperamos a que termine el proceso hijo
         wait(NULL);
    } 
}




void tuberias(char* cadena) {
    int longitud = strlen(cadena);
    int tubo_atras = 0;
    int tubo_adelante = 0;
    char comando[longitud];
    int i = 0;
    for (int j = 0; j <= longitud; j++) {
        if (cadena[j] == '|' || cadena[j] == '\0') {
            comando[i] = '\0';
            printf("comando: \"%s\" ", comando);
            if (tubo_atras) {
                printf("con tubería anterior ");
            } else {
                printf("sin tubería anterior ");
            }
            if (cadena[j] == '|') {
                tubo_atras = strlen(comando) + tubo_adelante + 1;
                tubo_adelante = 1;
                printf("y tubería siguiente\n");
            } else {
                tubo_atras = 0;
                tubo_adelante = 0;
                printf("sin tubería siguiente\n");
            }
            i = 0;
        } else {
            comando[i] = cadena[j];
            i++;
        }
    }
}




int main(){
    pipe(tuberia);
    //funcion(ejecucion de ls(1),poner en la tuberia,sacar de la tuberia)
    //(0=apagado,1=encendido)
    childmake(1,1,0,"ejecutando ls");
    //funcion(ejecucion de wc(2),poner en la tuberia,sacar de la tuberia)
    //(0=apagado,1=encendido)
    childmake(2,0,1,"ejecutando wc");
}