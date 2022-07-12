/*
    calculate the product of two matrices of dimensions (m * n and n * p) of integers
    dynamically allocate, making use of m concurrent threads.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <time.h>
#include <unistd.h>

#define M 4
#define N 3
#define P 2
int **matrix1,**matrix2, **matrix3; 
pthread_mutex_t mutex; 

void* product(void* args){
    
    pthread_mutex_lock(&mutex);
    int index = *((int*) args); 
    for(int i = 0; i<P; i++){
        for(int j = 0; j<N; j++){
            matrix3[index][i] += matrix1[index][j] * matrix2[j][i]; 
        }
    }
    pthread_mutex_unlock(&mutex); 
    return NULL; 
}

int main(int argc, char** argv){
    pthread_mutex_init(&mutex,NULL);
    int i,j; 
    srand(time(NULL));  
    pthread_t threads[M]; 


    matrix1 = (int**)malloc(M*sizeof(int*));
    for(i=0; i<M; i++)
        matrix1[i]=(int*)malloc(N*sizeof(int)); 


    matrix2 = (int**)malloc(N*sizeof(int*));
    for(i=0; i<N; i++)
        matrix2[i]=(int*)malloc(P*sizeof(int)); 


    matrix3 = (int**)malloc(M*sizeof(int*));
    for(i=0; i<M; i++)
        matrix3[i]=(int*)malloc(P*sizeof(int)); 


    for(i=0; i<M; i++){
        for(j=0; j<N; j++){
            matrix1[i][j] = rand() % 10 +1; 
        }
    }

     for(i=0; i<N; i++){
        for(j=0; j<P; j++){
            matrix2[i][j] = rand() % 10 +1; 
        }
    }

    int* k; 
    for(i=0; i<M; i++){
        k = (int*)malloc(sizeof(int)); 
        *k=i; 
    pthread_create(&threads[i],NULL,product,k); 
  
    }

     for(i=0; i<M; i++)
    pthread_join(threads[i],NULL); 


    printf("\n___MATRIX PRINTER___\n\n"); 
    printf("MATRIX 1\n\n"); 
    for(i=0; i<M; i++){
        for(j=0; j<N; j++){
            printf("%d\t",matrix1[i][j]); 
        }
        printf("\n"); 
    }
     printf("\nMATRIX 2\n\n"); 
    for(i=0; i<N; i++){
        for(j=0; j<P; j++){
            printf("%d\t",matrix2[i][j]); 
        }
        printf("\n"); 
    }
     printf("\nMATRIX 3\n\n"); 
    for(i=0; i<M; i++){
        for(j=0; j<P; j++){
            printf("%d\t",matrix3[i][j]); 
        }
        printf("\n"); 
    }

    pthread_mutex_destroy(&mutex); 
    return 0; 
}