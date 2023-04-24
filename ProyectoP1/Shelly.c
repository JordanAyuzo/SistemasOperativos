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
/*---Proyecto Shelly(MiniShell)---*/
/*Area de Variables Globlaes*/
int tuberia[2];

/* Con esto obtienes la ruta*/
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
/*Obtienen el nombre del comando*/
char* get_command_name(char *cadena) {
    char *token = strtok(cadena, " ");
    if (token != NULL) {
        return token;
    } else {
        return NULL;
    }
}
/*Busca el Path donde se encuentra el comando*/
/*Divide la cadena en un arreglo para enviar a las funciones exec*/
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
/*Funcion para hacer procesos Hijos*/
void childmake(char* path,char** args,int in,int out, char* nombreComando){  
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
        execv(path,args);
        printf("\033[31m Comando \033[33m\"%s\"\033[31m no reconocido.Intenta con otro comando\033[0m\n",nombreComando);
        exit(-1);
    }else{
        //esperamos a que termine el proceso hijo
         wait(NULL);
    } 
}
/*esta funcion es cuando existe una tuberia*/
void tuberias(char* cadena) {
    printf("%s\n",cadena);
    int longitud = strlen(cadena);
    int tubo_atras = 0;
    int tubo_adelante = 0;
    int tubo_atras1 = 0;
    int tubo_adelante1 = 0;
    char comando[longitud];
    int i = 0;
    for (int j = 0; j <= longitud; j++) {
        if (cadena[j] == '|' || cadena[j] == '\0') {
            comando[i] = '\0';
            if (tubo_atras)
                tubo_atras1 = 1;
            else 
                tubo_atras1 = 0;
            if (cadena[j] == '|') {
                tubo_atras = strlen(comando) + tubo_adelante + 1;
                tubo_adelante1 = 1;
            } else {
                tubo_adelante1 = 0;
                tubo_atras = 0;
                tubo_adelante = 0;
            }
            char pathi[MAX_PATH_LENGTH];
            strcpy(pathi,"/bin/");
            char** args = dividir_cadena(comando);
            //obtenemos el nombre del comando      cadena = (ls)
            char *comandito = get_command_name(comando);
            //unimos bin con el comando   /bin/comando
            strcat(pathi,comandito); 
            // aniadimos el comando a nuestro arreglo de parametros
            args = realloc(args, sizeof(char*) * 2);
            args[0] = pathi;
            //ejecutamos el comando
            printf("%s , %s , %d , %d , %s \n",pathi,args[1],tubo_adelante1,tubo_atras1,comandito);
                childmake(pathi,args,tubo_atras1,tubo_adelante1,comandito);
            i = 0;
        } else {if(cadena[j] == '>' ||cadena[j] == '<' || cadena[j] == '\0'){
                        printf("pilas ahi cuate\n");
                        comando[i] = '\0';
                        printf("comando: \"%s\" ", comando);
                    }else{
                        comando[i] = cadena[j];
                        i++;
                    }
        }
    }
}
//
void prueba(){
    printf("si llega \n");
    char *args1[] = { "/bin/ls", "-l", NULL };
    childmake(args1[0],args1,0,1,"ls");
    printf("hizo esta mamada");
    char *args2[] = { "/bin/wc", NULL };
    childmake(args2[0],args2,1,0,"wc");
}

/*Codigo Principal*/
int main(){
    pipe(tuberia);
    char cwd[MAX_PATH_LENGTH]; //para guardar el path
    char* cadena; //almacena todas los caracteres
    char path[MAX_PATH_LENGTH];  //aqui sera donde guardaremos el path del ejecutable
    //char* path = "/usr/bin/ls";
    size_t longitud = 0; //tama;o de la cadena que pienso recibir, 0 para que sea dinamico
    ssize_t bytes_leidos;   //variable para saber si la cadenna se ha leido bien o hubo error
    system("clear");//limpia consola en ANSI c con printf("\033[2J\033[1;1H");
    while (1){
        strcpy(path,"/bin/");
        //imprimimos en donde nos encontramos y leemos el comando
        printf("\033[36m>> ");
        printf("\033[1;34m%s\033[0m ", getcwd(cwd, sizeof(cwd)));
        printf("$ \033[1;32m");
        bytes_leidos = getline(&cadena, &longitud, stdin);
        //le quitamos el \n del final a nuestra cadena
        cadena[strcspn(cadena, "\n")] = '\0'; 
        char *cadenacopia = strdup(cadena);
        char* cadenacopia2= strdup(cadena);; //almacena todas los caracteres
        char* cadenacopy= strdup(cadena);; //almacena todas los caracteres
        // si la cadena es vacía o de espacios en blanco, saltamos a la siguiente iteración del ciclo
        if (strspn(cadena, " ") == strlen(cadena)) 
            continue;
        if (bytes_leidos == -1) {
            // el if es si hay un problema en la lectura suele suceder poco
            printf("\033[31mHay algun problema al leer esto. Intentalo de nuevo!\033[0m\n");
            return 1;
        }else{
            //checa si tiene cd ya que es una excepcion de comando
            if (strcmp(strtok(cadena, " "), "cd") == 0 ){
                if(chdir(obtener_ruta(cadenacopia+3)) != 0)
                    perror("\033[31cdmfailed");
            continue;
            }
            //vemos si existe tuberias(|,<,>)
            if ( strchr(cadenacopia, '|') != NULL || strchr(cadenacopia, '>') != NULL || strchr(cadenacopia, '<') != NULL) {
                printf("se lleva a cadena copia: %s \n",cadenacopia2);
                //tuberias(cadenacopia);
                prueba();
            }else{ 
                //si entra a esta condicional sabemos que es un comando simple
                //sabemos que para que se ejecute la salida del shell con que lleve exit es mas que suficiente
                if (strcmp(cadena, "exit") == 0)
                    exit(0);
                 //preparamos la cadena para la ejecucion del comando
                //verificar porque esta modificando a cadena = (ls -l)
                char** args = dividir_cadena(cadenacopy);
                //obtenemos el nombre del comando      cadena = (ls)
                char *comando = get_command_name(cadenacopy);
                //unimos bin con el comando   /bin/comando
                strcat(path,comando); 
                // aniadimos el comando a nuestro arreglo de parametros
                args = realloc(args, sizeof(char*) * 2);
                args[0] = path;
                //ejecutamos el comando
                childmake(path,args,0,0,comando);
            }
        }   
    }
    free(cadena); //libera la memoria asignada a cadena
    return 0; 
}