#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define TOTAL_CLIENTES 20
#define SILLON_CAPACIDAD 3
////sem_t //sem_sillon;
//sem_t //sem_barbero;

typedef struct {
    int capacity;
    int front;
    int rear;
    char** array;
} Queue;
struct thread_args {
    char* nombre;
    Queue* sillon;
};
Queue* create_queue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = -1;
    queue->rear = -1;
    queue->array = (char**)malloc(capacity * sizeof(char*));
    return queue;
}

int is_full(Queue* queue) {
    return (queue->rear == queue->capacity - 1);
}

int is_empty(Queue* queue) {
    return (queue->front == -1);
}

void enqueue(Queue* queue, char* item) {
    if (is_full(queue)) {
        printf("Cola llena, cliente %s se fue sin atender.\n", item);
        return;
    }
    if (is_empty(queue)) {
        queue->front = 0;
    }
    queue->rear++;
    queue->array[queue->rear] = item;
    printf("Cliente %s se ha sentado en la cola.\n", item);
}

char* dequeue(Queue* queue) {
    if (is_empty(queue)) {
        printf("La cola está vacía.\n");
        return NULL;
    }
    char* item = queue->array[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front++;
    }
    return item;
}

void* barbero(void* arg) {
    Queue* sillon = (Queue*)arg;
    while (1) {
        
        while (is_empty(sillon)) {
            printf("El barbero está durmiendo...\n");
            sleep(1);
        }
        //sem_wait(&//sem_sillon);
        char* cliente = dequeue(sillon);
        //sem_post(&//sem_barbero);
        printf("El barbero está atendiendo a %s.\n", cliente);
        sleep(1);
        printf("El barbero ha terminado con %s.\n", cliente);
    }
}


void* cliente(void* arg) {
    struct thread_args* args = (struct thread_args*)arg;
    char* nombre = args->nombre;
    Queue* sillon = args->sillon;
    printf("Cliente %s ha llegado a la barbería.\n", nombre);
    //sem_wait(&//sem_barbero);
    enqueue(sillon, nombre);
    //sem_post(&//sem_sillon);
    sleep(1);
}

int main() {

    //sem_init(&//sem_sillon, 0,0);
    //sem_init(&//sem_barbero, 0, 0);
    
    Queue* sillon = create_queue(SILLON_CAPACIDAD);
    pthread_t barbero_t;
    pthread_create(&barbero_t, NULL, barbero, (void*)sillon);
    pthread_t cliente_t[TOTAL_CLIENTES];
    char* nombres[] = {"Pedro", "Javier", "Rosita", "Patricia", "Olivia", "Raúl", "Brian", "David", "Roberto", "Luis", "Amanda", "Miguel", "Juan", "María", "Lucía", "Carlos", "Fernanda", "Andrés", "Daniela", "Ana"};
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        struct thread_args args = {nombres[i], sillon};
        pthread_create(&cliente_t[i], NULL, cliente,(void*)&args);
        sleep(1);
    }
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        pthread_join(cliente_t[i], NULL);
    }
    return 0;
}
