
#include <stdio.h>
#include <string.h>
void tuberias(char* cadena) {
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
            printf("coamndo %s entrada%d  salida%d \n",comando ,tubo_atras1,tubo_adelante1);
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

int main() {
    char cadena[] = "ls -l path| wc |cd|algo|algito nada";
    tuberias(cadena);
    return 0;
}