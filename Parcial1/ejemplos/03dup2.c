#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
//duplica la salida que necesites a la salida estandar
int main(){
    int fd = open("archivo2.txt", O_RDONLY|O_CREAT|O_TRUNC,0600);
    dup2(fd,1);
    close(fd);
    printf("hola mundo\n");
    return 0;
}