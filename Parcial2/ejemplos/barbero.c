#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define DORMIDO 0
#define HAY_SILLAS 1
#define CLIENTES_ESPERANDO 0


#define TOTAL_CLIENTES 10


void *barbero(void *s);
void *cliente(void *s);
void cortarPelo();


sem_t barberoListo;       // Semáforo barberoListo = 0  Dormido o barberoListo
sem_t haySillas;          // Semáforo haySillas = 1. Cuando sea 1, el número de sillas libres puede
                         // aumentar o disminuir
sem_t clientes_esperando; // Número de clientes esperando en la sala de espera
int sillasLibres = 4;     // N es el número total de sillas


int main()
{
   char *nombres[] = {"Pedro", "Javier", "Rosita", "Patricia", "Olivia", "Raúl", "Brian", "David", "Roberto", "Luis"};
   pthread_t peluquero;
   pthread_t melenudo[TOTAL_CLIENTES];


   // barbero inicia dormido
   sem_init(&barberoListo, 0, DORMIDO);
   // inicia con sillas de espera disponibles
   sem_init(&haySillas, 0, HAY_SILLAS);
   // de inicio, no hay clientes esperando
   sem_init(&clientes_esperando, 0, CLIENTES_ESPERANDO);


   pthread_create(&peluquero, NULL, barbero, NULL);
   for (int k = 0; k < TOTAL_CLIENTES; k++)
       pthread_create(&melenudo[k], NULL, cliente, (void *)nombres[k]);


   pthread_join(peluquero, NULL);
   for (int k = 0; k < TOTAL_CLIENTES; k++)
       pthread_join(melenudo[k], NULL);
  
   exit(0);
}


void *barbero(void *b)
{
   while (1)
   {
       sem_wait(&clientes_esperando); // espera que un cliente lo despierte
       sem_wait(&haySillas);          // ya esta despierto. debe aumentar
       sillasLibres++;                // el numero de silla libres.
       sem_post(&barberoListo);       // esta listo para cortar. le avisa al cliente.
       sem_post(&haySillas);          // deja que otros modifique sillas libres.
                                      // cortarPelo();
   }
   pthread_exit(NULL);
}


void *cliente(void *name)
{
   sem_wait(&haySillas); // candado de sillasLibres
   if (sillasLibres > 0)
   {
       printf("Sillas libres: %d\n", sillasLibres);
       sillasLibres--;                // disminuye la cantidad de sillas libres.
       sem_post(&clientes_esperando); // le avisa al barbero que hay un cliente disponible.
       sem_post(&haySillas);          // libera la variable sillasLibres
       sem_wait(&barberoListo);       // espera a que el barbero esté listo.
