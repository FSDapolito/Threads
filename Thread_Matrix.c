/*
     Create m threads and a matrix mxn, this threads have to do partial sum on each rows.
     Each thread after calculate its partial sum, wait two second before update total sum.
     Another thread is waiting all the threads'task, at the end it checks each thread's partial sum and the final result.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define M 4
#define N 4

int PartialSum[M];
int FinalResult =0; 
int **matrix; 
pthread_mutex_t mutex,mutex1; 
pthread_cond_t condizione = PTHREAD_COND_INITIALIZER; 
int attesa = 0;

void* sommeParz(void* args){

        int indice = *((int*)args);
        PartialSum[indice]=0; 


        pthread_mutex_lock(&mutex);       
        PartialSum[attesa]=0; 
        int k; 
        for(k=0; k<N; k++){
            PartialSum[attesa]+= matrix[indice][k];   
        }
        printf("PARTIAL SUM OF THREAD [%d]---> %d\n",attesa,PartialSum[attesa]); 
        
        pthread_mutex_unlock(&mutex);

        sleep(2);
        FinalResult = FinalResult + PartialSum[attesa]; 

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
      

        printf("\nALL THREADS TERMINATED\n"); 
        printf("\nTOTAL SUM ---> %d\n",FinalResult); 
        
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
