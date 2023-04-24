//https://man7.org/linux/man-pages/man7/signal.7.html


#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signalNumber){
   printf("la notificacion recibida tiene el numero: %d\n", signalNumber);
}

int main(){
   signal(SIGIO, &signalHandler);
   
   while( 1 ){
      printf("durmiendo\n");
      sleep(2);
   }   
   return 0;
}