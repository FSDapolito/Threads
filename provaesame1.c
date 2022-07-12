/*
*   SCRIVERE UN PROGRAMMA CHE CREI DUE THREAD CHE ESEGUONO LA FUNZIONE INCREMENTA
*   CHE A SUA VOLTA ACCEDE ALLE VARIABILI GLOB.A E GLOB.B DI UNA STRUTTURA DATI CONDIVISA
*   NE INCREMENTI IL LORO VALORE DI 1 PER 100 VOLTE. AL TERMINE, QUANDO I DUE THREAD AVRANNO TERMINATO
*   CON GLI INCREMENTI, IL THREAD PRINCIPALE STAMPERA' A VIDEO I VALORI DELLE VARIABILI TEST.A E TEST.B. 
*   PER LA GESTIONE DELLA SINCRONIZZAZIONE SI UTILIZZINO I MUTEX ALLOCATI DINAMICAMENTE
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct {
        int a; 
        int b; 
}glob; 

pthread_mutex_t mutex; 

void* incrementa(void* args)
{
    for(int k = 0; k<50; k++){
        pthread_mutex_lock(&mutex); 
        glob.a++; 
        glob.b++; 
        pthread_mutex_unlock(&mutex); 
    }
    return NULL; 
}

int main()
{
    pthread_mutex_init(&mutex,NULL); 
    glob.a = 0; 
    glob.b = 0; 
    pthread_t threads[2]; 
    
    for(int i=0; i<2; i++)
        pthread_create(&threads[i], NULL,incrementa, NULL); 

    for(int j = 0; j<2; j++)
        pthread_join(threads[j],NULL); 

    printf("I VALORI DELLE DUE VARIABILI SONO\n A = %d\t B = %d\n",glob.a, glob.b); 

    pthread_mutex_destroy(&mutex); 
    return 0; 
}