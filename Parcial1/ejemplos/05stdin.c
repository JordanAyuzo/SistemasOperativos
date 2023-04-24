#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
//duplica la salida que necesites a la salida estandar
int main(){
    int var;
    int fd = open("/archivo.txt",O_RDONLY);
    dup2(fd,0);     
    scanf("%i", &var);
    printf("%i",var);
}