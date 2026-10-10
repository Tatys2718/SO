#include <stdio.h>   
#include <unistd.h>     
#include <stdlib.h>     
#include <pthread.h>
#include <string.h>

typedef struct{
    int account_id;
    int balance;
}Cuenta;

typedef struct{
    int tx_id;
    int from_account;
    int to_account;
    int amount;
}Transferencia;

typedef struct {
    int tam;
    Transferencia *transfer;
}Bloque;

typedef struct {
    int id;
    int inicio;
    int fin;
    Bloque *bloque;
}Thread_args;

pthread_barrier_t barrier;

void read_file (const char *file);
void *func(void *arg);

Cuenta *counts;
Bloque *block;


int A;
int L;
int main (int argc, char **argv){
    if(argc < 2){
        perror("Error al leer el archivo");
    }

    read_file(argv[1]);

    int n_threads = atoi(argv[2]);
    
    printf("%d ", A);
    printf("%d\n", L);

    for(int i = 0; i < A; i++){
        printf("%d %d\n", counts[i].account_id, counts[i].balance);
    }

    for(int i = 0; i < L; i++){
        printf("%d\n", block[i].tam);
        for(int j = 0; j < block[i].tam; j++){
            printf("%d %d %d %d\n", block[i].transfer[j].tx_id, block[i].transfer[j].from_account, block[i].transfer[j].to_account, block[i].transfer[j].amount);
        }
    }

    printf("El numero de hilos ingresados es: %d \n", n_threads);

    pthread_t threads[n_threads];
    Thread_args args[n_threads];

    int por_hilo = block[0].tam / n_threads;
    int extra = block[0].tam % n_threads;

    int inicio = 0;

    pthread_barrier_init(&barrier, NULL, n_threads);

    for (int i = 0; i < n_threads; i++) {
        args[i].id = i;
        pthread_create(&threads[i], NULL, func, &args[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < L; i++){
        free(block[i].transfer);
    }
    pthread_barrier_destroy(&barrier);

    free(counts);
    counts = NULL;
}
void read_file (const char *file){
    FILE *f = fopen(file, "r");

    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    fscanf(f, "%d ", &A);
    fscanf(f, "%d\n", &L);

    counts = (Cuenta *) malloc(A * sizeof(Cuenta));
    if(!(counts)){
        perror("Fail malloc 1\n");
        exit(1);
    }

    block = (Bloque *) malloc(L * sizeof(Bloque));
    if(!(block)){
        perror("Fail malloc 1\n");
        exit(1);
    }


    for(int i = 0; i < A; i++){
        fscanf(f, "%d %d\n", &counts[i].account_id, &counts[i].balance);
    }

    for(int i = 0; i < L; i++){
        fscanf(f, "%d", &block[i].tam);
        block[i].transfer = (Transferencia *) malloc(block[i].tam * sizeof(Transferencia));
        if(!(block[i].transfer)){
            perror("Fail malloc 1\n");
            exit(1);
        }
        for(int j = 0; j < block[i].tam; j++){
            fscanf(f, "%d %d %d %d\n", &block[i].transfer[j].tx_id, &block[i].transfer[j].from_account, &block[i].transfer[j].to_account, &block[i].transfer[j].amount);
        }
    }

    fclose(f);
}
void *func(void *arg){
    Bloque *thread = (Bloque*) arg;
    for(int i = 0; i < ;){

    }
}