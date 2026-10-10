#include <stdio.h>
#include <stdlib.h>

int frecuencia(int n, int **matrix, int fila, int col);
int main (int argc, char **argv){
    int **matrix;
    int fila;
    int col;
    int frecuency = 0;
    int n = atoi(argv[1]);
    FILE *f = fopen(argv[2], "r");
    if(f == NULL){
        perror("Error al abrir el archivo \n");
        return 1;
    }
    fscanf(f, "%d", &fila);
    fscanf(f, "%d", &col);
    
    matrix = (int **) malloc (fila * sizeof (int *));
    if(!matrix){
        printf("Error en malloc1. \n");
        return 1;
    }
    for(int i = 0; i < fila; i++){
        matrix[i] = (int *) malloc(col * sizeof (int));
        if(!matrix){
            printf("Error en malloc 2. \n");
        }
    }

    for(int i = 0; i < fila; i++){
        for(int j = 0; j < col; j++){
            fscanf(f, "%d", &matrix[i][j]);
        }
    }
    
    frecuency = frecuencia(n, matrix, fila, col);
    printf ("La frecuencia es: %d\n", frecuency);



    for(int i = 0; i < fila; i++){
        free(matrix[i]);
    }
    free(matrix);
    fclose (f);
    return 0;
}
int frecuencia (int n, int **matrix, int fila, int col){
    int contador = 0;
    for(int i = 0; i < fila; i++){
        for(int j = 0; j < col; j++){
            if (matrix [i][j] == n){
                contador = contador + 1;
            }
        }
    }
    return contador;
}