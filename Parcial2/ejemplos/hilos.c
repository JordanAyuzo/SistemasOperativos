#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
//las funciónes que represetnesn a los hilos debe ser de tipo void *, y sólo aceptan un parámetro
//de tipo void *
void *proceso_1 (void * s){
   printf("Soy un proceso delgado mejor conocido como Hilo o Thread\n");
   pthread_exit(NULL);
}
void *proceso_2(void *parametro){
   printf("Yo soy otro Hilo o Thread\n");
   int *ptr = (int *) parametro;
   printf("mi dato de entrada es %d \n", *ptr);
   pthread_exit(NULL);
}
int main(){
   int k = 25;
   pthread_t hilo_1, hilo_2;
   pthread_create(&hilo_1, NULL, proceso_1, NULL);
   //p1 = dirección de una variable tipo hilo
   //p2 = NULL
   //p3 = dirección de una función (código del hilo en este caso)
   //p4 = NULL


   pthread_create(&hilo_2, NULL, proceso_2, NULL);
   pthread_join(hilo_1, NULL); //hace que el hilo principal espere a que el hilo 1 termine
   pthread_join(hilo_2, NULL);


   printf("fin de main\n");
   return 0;
}
