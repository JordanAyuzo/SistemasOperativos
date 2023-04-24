#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int n, char *argv[]){
    for(int j = 0; j < n; j++){
        fprintf(stdout, "%s ", argv[j]);
    }
    fprintf(stdout, "\n");
    return 0;
}
//./a.out lola beto juan 2023 56.89
/*
capas de la computadora 

user
shell          shell -Concha
 s.o
hw
*/