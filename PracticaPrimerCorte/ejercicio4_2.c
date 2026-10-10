#include <stdio.h>
#include <stdlib.h>

int frecuencia(int *vector, int n, int tam);

int main(int argc, char **argv){
    int *vector;
    int tam;
    int frecuency;

    int n = atoi(argv[1]);
    FILE *f = fopen(argv[2], "r");


    if(f == NULL){
        perror("Error al abrir el archivo");
        return 1;
    }

    fscanf(f, "%d", &tam);
    printf("El tamanio del vector es de: %d\n", tam);


    vector = (int *) malloc (tam * sizeof(int));

    for(int i = 0; i < tam ; i++){
        fscanf(f, "%d", &vector[i]);
    }

    frecuency = frecuencia(vector, n, tam);
    printf("La frecuencia es: %d\n", frecuency);

    free(vector);
    vector = NULL;

    fclose(f);
    return 0;

}
int frecuencia (int *vector, int n, int tam){
    int contador = 0;
    for(int i = 0; i < tam; i++){
        if(vector[i] == n){
            contador = contador + 1;
        }
    }
    return contador;
}