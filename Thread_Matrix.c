/*
     Scrivere un programma che, usando la libreria pthread,
     sommi gli elementi di una matrice mxn mediante m thread deputati al calcolo delle somme parziali su ciascuna riga. 
     Ogni thread, inoltre, calcolata la propria somma va ad aggiornare, dopo due secondi, la somma totale. 
     Un ulteriore thread resta in attesa che ciascun thread abbia finito e quindi riepiloga la somma parziale calcolata da ciascun thread ed il risultato complessivo.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define M 4
#define N 4

int sommeParziali[M];
int risultatoComplessivo =0; 
int **matrix; 
pthread_mutex_t mutex,mutex1; 
pthread_cond_t condizione = PTHREAD_COND_INITIALIZER; 
int attesa = 0;

void* sommeParz(void* args){

        int indice = *((int*)args);
        sommeParziali[indice]=0; 


        pthread_mutex_lock(&mutex);       
        sommeParziali[attesa]=0; 
        int k; 
        for(k=0; k<N; k++){
            sommeParziali[attesa]+= matrix[indice][k];   
        }
        printf("LA SOMMA PARZIALE del thread [%d]---> %d\n",attesa,sommeParziali[attesa]); 
        
        pthread_mutex_unlock(&mutex);

        sleep(2);
        risultatoComplessivo = risultatoComplessivo + sommeParziali[attesa]; 

        pthread_mutex_lock(&mutex1); 
        attesa++; 
        if(attesa==M){
            pthread_mutex_unlock(&mutex1); 
            pthread_cond_signal(&condizione); 
        }else{
            pthread_mutex_unlock(&mutex1); 
        }

    return NULL; 
}

void* riepilogo(){
    
         pthread_mutex_lock(&mutex1); 
        while(attesa != M)
            pthread_cond_wait(&condizione,&mutex1); 
      

        printf("\nTutti i thread hanno terminato\n"); 
        printf("\nLa somma totale ---> %d\n",risultatoComplessivo); 
        
        pthread_mutex_unlock(&mutex1); 

return NULL; 
}

int main(){
    pthread_t threads[M+1]; 
    srand(time(NULL)); 
    int i,j; 
    int* k; 
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex1,NULL);

    matrix=(int**)malloc(M*sizeof(int*)); 
    for(i=0; i<M; i++)
    matrix[i]=(int*)malloc(N*sizeof(int)); 

    for(i=0;i<M; i++){
        for(j=0; j<N; j++){
            matrix[i][j] = rand()%10; 
        }
    }

    printf("\n\n____MATRIX [%d][%d]____\n",M,N); 
     for(i=0;i<M; i++){
        for(j=0; j<N; j++){
           printf("%d\t ", matrix[i][j]); 
        }
        printf("\n"); 
    }

    for(i = 0; i<M; i++)
    {
            k=(int*)malloc(sizeof(int)); 
            *k=i; 
            pthread_create(&threads[i],NULL,sommeParz,(void*)k); 
            sleep(2); 
     }
        pthread_create(&threads[M+1],NULL,riepilogo,NULL); 

    for(i=0; i<=M; i++)
    pthread_join(threads[i],NULL); 

    pthread_mutex_destroy(&mutex1); 
    pthread_mutex_destroy(&mutex); 
    return 0; 

}
