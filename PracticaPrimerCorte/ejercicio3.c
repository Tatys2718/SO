#include<stdio.h>
#include<stdlib.h>

int main (int argc, char **argv){
    int n;
    int *vector;
    FILE *f = fopen(argv[1], "r");
    if(f == NULL){
        perror("Error al abrir el archivo. \n");
        return 1;
    }

    fscanf(f, "%d", &n);
    printf("%d\n", n);

    vector = (int *) malloc (n * sizeof(int));
    if(vector == NULL){
        printf("Error en malloc 1. \n");
    }

    for (int i = n-1; i >= 0 ; i --){
        fscanf(f, "%d", &vector[i]);
    }

    for(int i = 0; i < n; i ++){
        printf("%d \n", vector[i]);
    }

    free(vector);
    vector = NULL;
    fclose(f);
    return 0;
}