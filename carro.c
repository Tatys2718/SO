#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct{
    int cantidad;
    int capacidad;

    pthread_mutex_t mutex;

    pthread_cond_t lleno;
    pthread_cond_t vacio;

}Buffer;
typedef struct {
    int id;
    //fill
} thread_args;

void *func_chasis(void *arg);
void *func_motor(void *arg);
void *func_pintura(void *arg);

int main (int argc, char **argv){
    pthread_t *thread_chasis;
    pthread_t *thread_motor;
    pthread_t *thread_pintura;

    thread_args *args_chasis;
    thread_args *args_motor;
    thread_args *args_pintura;

    int n_chasis = 2;
    int n_motor = 3;
    int n_pintura = 2;

    thread_chasis = (pthread_t*) malloc(n_chasis * sizeof(pthread_t));

    if(!thread_chasis){
        perror("Fail malloc\n");
        exit(-1);
    }
    thread_motor = (pthread_t*) malloc(n_motor * sizeof(pthread_t));
    if(!thread_chasis){
        perror("Fail malloc\n");
        exit(-1);
    }
    thread_pintura = (pthread_t*) malloc(n_pintura * sizeof(pthread_t));
    if(!thread_motor){
        perror("Fail malloc\n");
        exit(-1);
    }

    args_chasis = (thread_args*) malloc(n_chasis * sizeof(thread_args));
    if(!args_chasis){
        perror("Fail malloc\n");
        exit(-1);
    }

    args_motor = (thread_args*) malloc(n_motor * sizeof(thread_args));
    if(!args_motor){
        perror("Fail malloc\n");
        exit(-1);
    }

    args_pintura = (thread_args*) malloc(n_pintura * sizeof(thread_args));
    if(!args_pintura){
        perror("Fail malloc\n");
        exit(-1);
    }

    for(int i = 0; i < n_chasis; i++){
        args_chasis[i].id = i;
        pthread_create(&thread_chasis[i], NULL, func_chasis, &args_chasis[i]);
    }

    for(int i = 0; i < n_motor; i++){
        args_motor[i].id = i;
        pthread_create(&thread_motor[i], NULL, func_motor, &args_motor[i]);
    }

    for(int i = 0; i < n_pintura; i++){
        args_pintura[i].id = i;
        pthread_create(&thread_pintura[i], NULL, func_pintura, &args_motor[i]);
    }


    for(int i=0; i < n_chasis; i++){
        pthread_join(thread_chasis[i], NULL);
    }

    for(int i=0; i < n_motor; i++){
        pthread_join(thread_motor[i], NULL);
    }

    for(int i=0; i < n_pintura; i++){
        pthread_join(thread_pintura[i], NULL);
    }

    free(thread_chasis);
    free(thread_motor);
    free(thread_pintura);
    free(args_chasis);
    free(args_motor);
    free(args_pintura);

}
void *func_chasis(void *arg){

}
void *func_motor(void *arg){
    
}
void *func_pintura(void *arg){
    
}