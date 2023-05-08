#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


typedef struct queue Queue;
typedef struct {
    Queue* queue;
    char *otro_argumento;
} Argumentos;

struct queue{
    int front, rear;
    unsigned capacity;
    int *array;
};

Queue* create_queue(unsigned capacity);
void destroy_queue(Queue *queue);
void enqueue(Queue *queue, int item);
int dequeue(Queue *queue);
int peek(Queue *queue);
void display(Queue *queue);
int is_full(Queue *queue);
int is_empty(Queue *queue);
int menu();
void *barbero(void *s);
void *cliente(void *s);
void cortarPelo();
#define DORMIDO 0
#define HAY_SILLAS 1
#define CLIENTES_ESPERANDO 0
#define TOTAL_CLIENTES 10

sem_t barberoListo;       // Semáforo barberoListo = 0  Dormido o barberoListo
sem_t haySillas;          // Semáforo haySillas = 1. Cuando sea 1, el número de sillas libres puede
                         // aumentar o disminuir
sem_t clientes_esperando; // Número de clientes esperando en la sala de espera


Queue* create_queue(unsigned capacity){
    Queue *queue = (Queue *) malloc (sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->rear = -1;
    queue->array = (int *) malloc ( queue->capacity * sizeof(int));
    return queue;
}
void destroy_queue(Queue *queue){
    free(queue->array);
    free(queue);
}
void enqueue(Queue *queue, int item){
    if(is_full(queue))
        printf("CLIENTE DESCONOCIDO: Ya no alcance llegar temprano\n");
    else 
        queue->array[++queue->rear] = item;
}
int dequeue(Queue *queue){
    if(is_empty(queue)){
        printf("BARBERO: Ya corri a todos, Me duermo en mi sillon\n");
        return INT_MIN;
    } else {
        return queue->array[++queue->front];
    }
}
int peek(Queue *queue){
    if(is_empty(queue)){
        printf("No hay clientes.\n");
        return 0;
    } else {
        return queue->array[queue->front + 1];
    }
}
void display(Queue *queue){
    if(is_empty(queue)){
        printf("\nCola esta vacía, no hay elementos que mostrar");
    } else {
        for (int i = queue->front + 1; i <= queue->rear; i++){
            printf("\n%d", queue->array[i]);
        }
        
    }
}
int is_full(Queue *queue){
    return (queue->rear == (int) queue->capacity - 1);
}
int is_empty(Queue *queue){
    return (queue->rear == -1);
}
int main(){
    int option, item;
    Queue *queue = create_queue(4);
     
    pthread_t peluquero;
    pthread_t melenudo[TOTAL_CLIENTES];
    // barbero inicia dormido
   sem_init(&barberoListo, 0, DORMIDO);
   // inicia con sillas de espera disponibles
   sem_init(&haySillas, 0, HAY_SILLAS);
   // de inicio, no hay clientes esperando
   sem_init(&clientes_esperando, 0, CLIENTES_ESPERANDO);
   pthread_create(&peluquero, NULL, barbero, (void*)&queue);
   Argumentos argumentos_melenudo[TOTAL_CLIENTES];
   char *nombres[] = {"Pedro", "Javier", "Rosita", "Patricia", "Olivia", "Raúl", "Brian", "David", "Roberto", "Luis"};
   for (int k = 0; k < TOTAL_CLIENTES; k++){
    argumentos_melenudo[k].queue = queue;
    argumentos_melenudo[k].otro_argumento = nombres[k];
    pthread_create(&melenudo[k], NULL, cliente, (void*)&argumentos_melenudo[k]);
   }
       

   pthread_join(peluquero, NULL);
   for (int k = 0; k < TOTAL_CLIENTES; k++)
       pthread_join(melenudo[k], NULL);
  
   exit(0);


    while(1){
        option = menu();
        switch (option)
        {
        case 1: printf(("Ingresa un elemento "));
                scanf("%d", &item);
                enqueue(queue, item);
                break;
        case 2: item = dequeue(queue);
                if(item != INT_MIN)
                    printf("Elemento retirado: %d", item);
                else
                    printf("Cola esta vacía");
                break;
        case 3: item = peek(queue);
                if(item == INT_MIN)
                    printf("No hay elementos en la cola");
                else
                    printf("Elemento al frente de la cola: %d", item);
                break;
        case 4: display(queue);
                break;
        case 5: destroy_queue(queue);
                exit(0);
        
        default:printf("\nOpción inválida");
            break;
        }
    }
}

int menu(){
  int option;
  printf("\n\n\n*** MENU COLA-QUEUE***\n");
  printf("\n1. Insertar elemento");
  printf("\n2. Eliminar elemento");
  printf("\n3. Mirar elemento de front");
  printf("\n4. Mostrar elementos de la cola");
  printf("\n5. Salir");
  printf("\nIngrear una opción:");
  
  scanf("%d", &option);
  return option;
}


void *barbero(void *b){
   Queue* queue = *((Queue**)b);
   while (1)
   {
        printf("BARBERO:(Busca Sillas Libres)\n");
       sem_wait(&clientes_esperando); // espera que un cliente lo despierte
       sem_wait(&haySillas);          // ya esta despierto. debe aumentar
        dequeue(queue);            // el numero de silla libres.
       sem_post(&barberoListo);       // esta listo para cortar. le avisa al cliente.
       sem_post(&haySillas);          // deja que otros modifique sillas libres.
                                      // cortarPelo();
   }
   pthread_exit(NULL);
}

void *cliente(void *b)
{
    
    Argumentos* argumentos = (Argumentos*)b;
    Queue* queue = argumentos->queue;
    char *nombre = argumentos->otro_argumento;  
    int mechudo=peek(queue);
   sem_wait(&haySillas); // candado de sillasLibres
   if (is_full(queue)==0)
   {
        enqueue(queue,1);
        // disminuye la cantidad de sillas libres.
       sem_post(&clientes_esperando); // le avisa al barbero que hay un cliente disponible.
       sem_post(&haySillas);          // libera la variable sillasLibres
       sem_wait(&barberoListo);       // espera a que el barbero esté listo.
        cortarPelo((char *)nombre);
   }else
   {
       printf("%s no encontro sillas libres\n", nombre);
       sem_post(&haySillas); // libera sillasLibres
   }
  
   pthread_exit(NULL);
}

void cortarPelo(char *s)
{
   printf("\tcortando el pelo a %s\n", s);
   sleep(10);
}