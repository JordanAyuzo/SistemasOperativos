#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
//duplica la salida que necesites a la salida estandar
int main(){
    char cadena;
    //proceso padre
    int tuberia[2];
    pipe(tuberia);

    // proceso 1 con fork
    dup2(tuberia[1],1);
    close(tuberia[1]);
    printf("hola mundo");

    //proceso 2
    dup2(tuberia[0],0);
    close(tuberia[0]);
    gets(cadena);//declarar variable
}
