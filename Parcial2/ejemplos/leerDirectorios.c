#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc ,char *argv[]){
     DIR *directorio;
    struct dirent *direntp;
    directorio = opendir(".");
    if(directorio==NULL){
        printf("algo anda mal\n");
        exit(2);
    }
    while ((direntp = readdir(directorio))!=NULL)
    {
        switch (direntp->d_type)
        {
        case DT_DIR:
            printf("directorio\n");
            break;
        case DT_REG:
            printf("Archivo regular\n");
            break;
        case DT_BLK:
            printf("dispositivo de bloque\n");
            break;
        case DT_CHR:
            printf("dispositivo de caracteres\n");
            break;
        case DT_FIFO:
            printf("Tuberia con nombre\n");
            break;
        case DT_SOCK:
            printf("socket\n");
            break;
        default:
            printf("\n");
            break;
        }
    }
}
