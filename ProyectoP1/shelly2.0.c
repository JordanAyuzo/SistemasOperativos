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
#define MAX_LENGTH 1024

int tuberia[2];
#define READ 0
#define WRITE 1


char *leerCadena();
char *obtener_ruta(char *cadenacopia);
void comandoSimple(char* cadena);
char** dividir_cadena(char* cadena);
char* obtenerComando(char* cadena);
void childmake(char* path,char** args,int in,int out, char* nombreComando);


//Funcion que se encarga de redireccionar

char* obtenerComando(char *cadena) {
    char *token = strtok(cadena, " ");
    if (token != NULL) {
        return token;
    } else {
        return NULL;
    }
}
char *obtener_ruta(char *cadenacopia) {
    char *ruta = NULL;
    char *delim = "/";
    char *token = strtok(cadenacopia, delim);
    int contador = 0;
    
    if (token == NULL) { // si el primer token es NULL, se ingresó solo "cd"
        ruta = malloc(2 * sizeof(char)); // reservamos espacio para la ruta "/"
        strcpy(ruta, "/");
        return ruta;
    }
    
    while (token != NULL) {
        if (contador == 0) {
            ruta = malloc((strlen(token) + 1) * sizeof(char));
            strcpy(ruta, token);
        } else {
            ruta = realloc(ruta, (strlen(ruta) + strlen(token) + 2) * sizeof(char));
            strcat(ruta, "/");
            strcat(ruta, token);
        }
        contador++;
        token = strtok(NULL, delim);
    }
    
    return ruta;
}
char* leerCadena(){
    char cwd[MAX_LENGTH]; //para guardar el path
    char* cadena; 
    size_t longitud = 0; 
    ssize_t bytes_leidos;
    printf("\033[36m>> ");
    printf("\033[1;34m%s\033[0m ", getcwd(cwd, sizeof(cwd)));
    printf("$ \033[1;32m");
    bytes_leidos = getline(&cadena, &longitud, stdin);
    cadena[strcspn(cadena, "\n")] = '\0'; 
     if (bytes_leidos == -1) {
        // el if es si hay un problema en la lectura suele suceder poco
         printf("\033[31mHay algun problema al leer esto. Intentalo de nuevo!\033[0m\n");
        return " ";
    }
    return cadena;
}
char** dividir_cadena(char* cadena) {
    // crear un arreglo de argumentos
    char** args = malloc(sizeof(char*));
    int num_args = 0;

    // dividir la cadena en argumentos y almacenarlos en el arreglo
    char* token = strtok(cadena, " ");
    while (token != NULL) {
        num_args++;
        args = realloc(args, sizeof(char*) * num_args);
        args[num_args - 1] = token;
        token = strtok(NULL, " ");
    }
    args = realloc(args, sizeof(char*) * (num_args + 1));
    args[num_args] = NULL; // terminar el arreglo de argumentos con un puntero nulo

    return args;
}
void comandoSimple(char* cadena){
    char path[MAX_LENGTH];
    strcpy(path,"/bin/");
    char* divCadena =strdup(cadena); 
    char** args = dividir_cadena(divCadena);
    char *comando =obtenerComando(cadena);
    strcat(path,comando); 
    args = realloc(args, sizeof(char*) * 2);
    args[0] = path;
    printf("comando %s\n",comando);
    printf("path %s\n",path);
    printf("arg 0 %s\n",args[0]);
    printf("arg 1 %s\n",args[1]);
    printf("arg 2 %s\n",args[2]);
    childmake(path,args,0,0,comando);
}
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
    system("clear");
    while (1){
        char* cadena = leerCadena();
        char* respaldoCadena =strdup(cadena);     
        if (strspn(respaldoCadena, " ") == strlen(respaldoCadena)){
            continue;
        }
        if (strcmp(strtok(respaldoCadena, " "), "cd") == 0 ){
            if(chdir(obtener_ruta(respaldoCadena+3)) != 0)
                perror("\033[31m cd failed");
        continue;
        }
        if (strcmp(respaldoCadena, "exit") == 0){
            exit(0);  
        }
        
        char* cadenaCompara = strdup(cadena); 
        char* entrada = strdup(cadena); 
        if ( strchr(cadenaCompara, '|') != NULL || strchr(cadenaCompara, '>') != NULL || strchr(cadenaCompara, '<') != NULL){
        
        
        }
         
        
        else{
            char* cadenaEnvia = strdup(cadena); 
            comandoSimple(cadenaEnvia);
        }
    }
}