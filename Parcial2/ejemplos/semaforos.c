/*
//Declaracion de la variable:
sem_t semaforo;
// inicalizacion del semaforo(esta funcion nunca poner negativos en N)
sem_int(&semaforo,0,N);
//decrementa el contador N
sem_wait(&semaforo) 
//incrementa el contador N
sem_post(semaforo);
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
int dato =10;
int N = 1;
        sem_t semaforo;
void *incrementa(void *p){
    for (int i = 0; i < 1000; i++)
    {
        sem_wait(&semaforo);
        dato++;
        sem_post(&semaforo);
    }
    pthread_exit(NULL);
}
void *decrementa(void *p){
    for (int i = 0; i < 1000; i++)
    {
        dato--;
    }
    pthread_exit(NULL);
}

int main(){

    sem_init(&semaforo,0,N);
    int k =25;

    pthread_t hilo_1, hilo_2;
    pthread_create(&hilo_1,NULL,incrementa,NULL);
    //pthread_create(&hilo_2,NULL,decrementa,NULL);
    pthread_join(hilo_1,NULL);
    //pthread_join(hilo_2,NULL);
    printf("dato = %i\n",dato);
    return 0;

}
