/*
Realizzare un programma C e Posix sotto Linux che, mediante l'ausilio della libreria pthread,
decomponga la somma degli elementi di un vettore di interi di dimensione 1000 in quattro somme locali
effettuate concorrentemente da altrettanti thread.
Si contempli l'uso di una variabile mutex per regolare l'accesso alla variabile globale 
che conterrà la somma degli elementi del vettore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

int vettore[1000]; 
int somma1=0; //0-249
int somma2=0; //250-499
int somma3=0; //500-749
int somma4=0; //750-999
pthread_mutex_t mutex; 

void* sum(void* args){
    int* j; 
    int i; 
    j=(int*)args; 

    if(*j==0){
        //primo thread
        printf("Sono il thread numero [%d]\n",*j+1); 
        pthread_mutex_lock(&mutex); 
        for(i=0; i<250; i++)
        somma1 = somma1+vettore[i]; 
        pthread_mutex_unlock(&mutex); 
    }
    else if(*j==1){
        //secondo thread
         printf("Sono il thread numero [%d]\n",*j+1); 
        pthread_mutex_lock(&mutex); 
        for(i=250; i<500; i++)
        somma2 = somma2+vettore[i]; 
        pthread_mutex_unlock(&mutex); 
    }
    else if(*j==2){
        //terzo thread
         printf("Sono il thread numero [%d]\n",*j+1); 
        pthread_mutex_lock(&mutex); 
        for(i=500; i<750; i++)
        somma3 = somma3+vettore[i]; 
        pthread_mutex_unlock(&mutex); 

    }
    else if(*j==3){
        //quarto thread
         printf("Sono il thread numero [%d]\n",*j+1); 
        pthread_mutex_lock(&mutex); 
        for(i=750; i<1000; i++)
        somma4 = somma4+vettore[i]; 
        pthread_mutex_unlock(&mutex); 
    }

    return NULL; 
}

int main(){
    pthread_mutex_init(&mutex,NULL); 
    pthread_t threads[4]; 
    int i;
    int* k;  
    srand(time(NULL)); 

    for(i=0; i<1000; i++)
        vettore[i]=rand()%100;

    for(i=0; i<4; i++){
        k=(int*)malloc(sizeof(int)); 
        *k=i; 
        pthread_create(&threads[i],NULL,sum,(void*)k); 
    }
    for(i=0; i<4; i++)
        pthread_join(threads[i],NULL); 
    pthread_mutex_destroy(&mutex); 
    sleep(1); 


    printf("La somma1 = %d\n",somma1); 
    printf("La somma2 = %d\n",somma2); 
    printf("La somma3 = %d\n",somma3); 
    printf("La somma4 = %d\n",somma4); 
    return 0; 
}