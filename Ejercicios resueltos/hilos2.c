//pretaller de galaxias
#include <stdio.h>   
#include <unistd.h>     
#include <stdlib.h>     
#include <pthread.h>
#include <string.h>

#define MAX_LEN 100

typedef struct {
    int id;
    int tam_galaxia;
    float *galaxia;
} thread_args;

int n_threads_recolector;
pthread_barrier_t barrier;

void read_file (const char *file, char ***vector, int *tam);
void create_threads(int n_threads);
void *func(void *arg);
void *func_analista(void *arg);

char **vector;
int tam;
int main (int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo porfavor.\n");
    }

    read_file(argv[1], &vector, &tam);

    printf("La cantidad de hilos recolectores es: %d \n", tam);

    pthread_barrier_init(&barrier, NULL, tam+1);

    create_threads(tam);

    thread_args vectores[tam];

    

    pthread_t hilo_analista;
    if(pthread_create(&hilo_analista, NULL, func_analista, (void*)vectores) != 0){
        perror("Error al crear el hilo analista");
        return 1;
    }
    
    pthread_join(hilo_analista, NULL);

    pthread_barrier_destroy(&barrier);
    free(vector);
    vector = NULL;

    return 0;
}
void read_file (const char *file, char ***vector, int *tam){
    FILE *f = fopen(file, "r");

    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }

    char buffer[MAX_LEN];

    fscanf(f, "%d\n", tam);

    *vector = (char**) malloc(*tam * sizeof(char *));

    if(!(*vector)){
        perror("Fallo malloc \n");
        exit(-1);
    }

    for(int i = 0; i < *tam; i++){
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        (*vector)[i] = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy((*vector)[i], buffer);
    }
    for(int i = 0; i < *tam; i++){
        printf("%s ---- y longitud %ld\n", (*vector)[i], strlen((*vector)[i]));
    }

    fclose(f);

}
void create_threads(int n_threads){
    pthread_t *threads; //n_threads
    thread_args *args;

    threads = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
    args = (thread_args*) malloc(n_threads * sizeof(thread_args));
    if(!threads || !args){
        perror("Fail malloc");
        exit(-1);
    }

    for(int i=0; i < n_threads; i++){
        args[i].id = i;
        pthread_create(&threads[i], NULL, func, &args[i]);
    }

    for(int i=0; i < n_threads; i++){
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(args);
}
void *func(void *arg){
    thread_args *thread = (thread_args*) arg;

    FILE *fl = fopen(vector[thread->id], "r");

    if(fl == NULL){
        perror("Error al abrir el archivo del hilo");
        exit(-1);
    }

    fscanf(fl, "%d\n", &thread->tam_galaxia);

    thread->galaxia = (float*) malloc(thread->tam_galaxia * sizeof(float));

    for(int i = 0; i < thread->tam_galaxia; i++){
        fscanf(fl, "%f\n", &thread->galaxia[i]);
    }

    printf("Hilo %d abrio y leyo el archivo %s\n", thread->id, vector[thread->id]);

    for(int i = 0; i < thread->tam_galaxia; i++){
        printf("Hilo %d -- %f \n", thread->id, thread->galaxia[i]);
    }

    fclose(fl);

    pthread_barrier_wait(&barrier);

    free(thread->galaxia);
    

    return NULL;
}

void *func_analista(void *arg){
    thread_args *vectores = (thread_args *)arg;
    printf("Entro el analista a esperar\n");
    pthread_barrier_wait(&barrier);
    printf("La barrera se rompio\n");
    float total = 0;
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < vectores[i].tam_galaxia; j++){
            total = total + vectores[i].galaxia[j];
        }
        float media = total / vectores[i].tam_galaxia;
        printf("La media de la galaxia del hilo %d es: %f\n", vectores[i].id, media);
    }

    return NULL;
}
