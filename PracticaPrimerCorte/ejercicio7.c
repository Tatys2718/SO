#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    int tam;
    int *vector = NULL;
    long int sum = 0;
    int max = 10000;

    FILE *f = fopen(argv[1], "r");

    if(f == NULL){
        perror("Error al abrir el archivo");
        return 1;
    }

    fscanf(f, "%d", &tam);
    printf("El tamanio del vector es de: %d\n", tam);

    vector = (int *)malloc(tam * sizeof(int));

    for(int i = 0; i < tam; i++){
        fscanf(f, "%d", &vector[i]);
        if(vector[i] <= max){
            sum = sum + vector[i];
        }else{
            perror("Los numeros del vector deben ser menores que 10k");
            break;
        }
        
    }

    printf("Sum = %ld\n", sum);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    free(vector);
    vector = NULL;

    fclose(f);
    return 0;
}