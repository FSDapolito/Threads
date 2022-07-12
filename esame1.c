/*
*   Scrivere un programma in C che prende da riga di comando N interi compresi tra 5 e 20. Il valore di N è costante.
*   Il programma avvia N thread che hanno il seguente comportamento: 
*   a) Ogni thread va in sospensione (invocando la funzione sleep) per un numero di secondi pari al numero del suo corrispettivo parametro e poi stampa il suo indice
*   b) Alla fine del periodo di attesa sleep(), ogni thread stampa un messaggio : "Thread # svegliato !"
*   c) Tutti i thread si sincronizzano tra di loro (utilizzando i semafori basati su memoria)
*   d) Dopo aver atteso il termine di tutti gli altri thread, ciascun thread scrive sullo shermo che ha terminato ("Thread #: terminato...").
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


pthread_cond_t condizione = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
sem_t  semaforo1,semaforo2; 
int sveglia = 0; 
int n = 0; 
int indice; 

void* routine(void *args); 
int main(int argc, char** argv){

    if(argc != 2){
        printf("Non hai inserito il valore n da riga di comando! Range [5,20]\n");
        exit(0); 
    }
    n = atoi(argv[1]); 
    if (n < 5 || n > 20){
        printf("Il valore inserito non rientra nel Range [5,20]\n"); 
        exit(0); 
    }


        sem_init(&semaforo1, 0, 1);
        sem_init(&semaforo2, 0, 1); 

        pthread_t threads[n]; 
        int i; 
        int *k; 
        for(i=0; i<n; i++)
        {
             k = (int*)malloc(sizeof(int)); 
            *k = i; 
            sleep(i); 
            pthread_create(&threads[i], NULL, routine, (void*)k); 
        } 

        for(i=0; i<n; i++)
            pthread_join(threads[i], NULL); 
        
        sem_destroy(&semaforo1); 
        sem_destroy(&semaforo2); 
      

        return 0; 




    return 0; 
}
void* routine(void *args){
        
        sem_wait(&semaforo1);
        indice = *((int*)args);
        sleep(indice+1); 
        printf("Thread [%d] svegliato\n",indice+1);
        sem_post(&semaforo1); 

        sem_wait(&semaforo2); 
        sveglia++; 
        sem_post(&semaforo2); 

        pthread_mutex_lock(&mutex);
        if(sveglia == n){
            pthread_cond_broadcast(&condizione);
                printf("Thread #: terminato....\n"); 
        }else{
            //l'ultimo sveglia tutti
                pthread_cond_wait(&condizione,&mutex); 
                printf("Thread #: terminato....\n"); 
        }
        pthread_mutex_unlock(&mutex);

    return NULL; 
}