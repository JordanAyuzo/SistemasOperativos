#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
int here = 0;
int count= 7;
void* miReloj(void *p){
    count ++;
}
void* bos(void *p){
    printf("Hola Mundo\n");
    here++;
    pthread_exit(NULL);
}
void*person1(void *p){
    if(here==1){
        if(count>9)
            printf("perdon Jefe -- Hora %i \n",count);
        else
            printf("Buenas  Jefe-- Hora %i",count);
    }
    else{
        printf("Wenas Wenas -- Hora %i \n",count);
    }
}
int main(){
    pthread_t hilo_1, hilo_2,hilo_jefe,hilo_3,hilo_tempo1,hilo_tempo2,hilo_tempo3;
    pthread_create(&hilo_tempo1,NULL,miReloj,NULL);
    pthread_create(&hilo_tempo2,NULL,miReloj,NULL);
    pthread_create(&hilo_tempo3,NULL,miReloj,NULL);
    pthread_create(&hilo_1,NULL,person1,NULL);
    pthread_create(&hilo_jefe,NULL,bos,NULL);
    pthread_create(&hilo_2,NULL,person1,NULL);
    pthread_create(&hilo_3,NULL,person1,NULL);
    pthread_join(hilo_1,NULL);
    pthread_join(hilo_2,NULL);
    pthread_join(hilo_3,NULL);
    pthread_join(hilo_jefe,NULL);
    pthread_join(hilo_tempo1,NULL);
    pthread_join(hilo_tempo2,NULL);
    pthread_join(hilo_tempo3,NULL);
    return 0;

}
