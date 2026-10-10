#include <stdio.h>
#include <stdlib.h>

int suma(int *vector, int tam);
int main(int argc, char **argv){
    int n;
    int suma_total;
    float promedio;
    int *vector;

    FILE *f = fopen(argv [1], "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
        return 1;
    }

    fscanf(f, "%d", &n);
    printf("%d\n", n);

    vector = (int *) malloc (n * sizeof(int));
    if(vector == NULL){
        printf("Error en malloc 1.\n");
    }
    for(int i = 0; i < n; i ++){
        fscanf(f, "%d", &vector[i]);
    }

    for(int i = 0; i < n; i++){
        printf("%d\n", vector[i]);
    }

    printf ("La cantidad total de numeros es: %d\n", n);
    suma_total = suma(vector, n);
    printf ("La suma total de los numeros es: %d\n", suma_total);
    promedio = suma_total/n;
    printf("El promedio es: %f\n", promedio);


    free(vector);
    vector = NULL;
    fclose(f);
    return 0;
}
int suma(int *vector, int tam){
    int total = 0;
    for (int i = 0; i < tam; i++){
        total = total + vector[i];
    }
    return total;
}