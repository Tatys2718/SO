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

pthread_barrier_t barrier;

void read_file (const char *file, char ***vector, int *tam);
void *func(void *arg);
void *func_analista(void *arg);
void quicksort(float *vector, int start, int end);
int partition (float *vector, int start, int end);


char **vector;
int tam;
int main (int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo porfavor.\n");
    }

    read_file(argv[1], &vector, &tam);
    printf("La cantidad de hilos recolectores es: %d \n", tam);

    pthread_t threads_recolec[tam];
    thread_args *args;

    args = (thread_args*) malloc(tam * sizeof(thread_args));

    pthread_barrier_init(&barrier, NULL, tam+1);

    for (int i = 0; i < tam; i++) {
        args[i].id = i;
        pthread_create(&threads_recolec[i], NULL, func, (void*)&args[i]);
    }

    //thread_args vectores[tam];
    pthread_t thread_analista;

    if(pthread_create(&thread_analista, NULL, func_analista, (void*)args) != 0){
        perror("Error al crear el hilo analista");
        return 1;
    }

    for (int i = 0; i < tam; i++) {
        pthread_join(threads_recolec[i], NULL);
    }

    pthread_join(thread_analista, NULL);
    
    for(int i = 0; i < tam; i++){
        free(args[i].galaxia);
    }

    
    pthread_barrier_destroy(&barrier);
    free(vector);
    vector = NULL;
    free(args);
    args = NULL;
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

void *func_analista(void *arg){
    thread_args *vectores = (thread_args *)arg;
    printf("Entro el analista a esperar\n");

    pthread_barrier_wait(&barrier);

    printf("La barrera se rompio\n");

    for(int i = 0; i < tam; i++){
        float total = 0;
        for(int j = 0; j < vectores[i].tam_galaxia; j++){
            printf("Estoy recorriendo en hilo %d en el numero %f\n", vectores[i].id , vectores[i].galaxia[j]);
            total = total + vectores[i].galaxia[j];
        }
        float media = total / vectores[i].tam_galaxia;
        printf("La media de la galaxia del hilo %d es: %f\n", vectores[i].id, media);
    }
    
    for(int i = 0; i < tam; i++){
        quicksort(vectores[i].galaxia, 0, vectores[i].tam_galaxia-1);
    }

    for(int i = 0; i < tam; i++){
        for(int j = 0; j < vectores[i].tam_galaxia; j++){
            printf("hilo (%d) -- %f \n", vectores[i].id, vectores[i].galaxia[j]);
        }
    }

    return NULL;
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

    //free(thread->galaxia);

    return NULL;
}

void quicksort(float *vector, int start, int end){
    if(end <= start){
        return;
    }
    int pivot = partition (vector, start, end);
    quicksort(vector, start, pivot - 1);
    quicksort(vector, pivot+1, end);

}
int partition (float *vector, int start, int end){
    float pivot = vector[end];
    int i = start-1;
    for(int j = start; j<= end -1; j++){
        if(vector[j]<pivot){
            i++;
            float temp = vector[i];
            vector[i] = vector[j];
            vector[j] = temp;
        }
    }
    i++;
    float temp = vector[i];
    vector[i] = vector[end];
    vector[end] = temp;
    return i;
}