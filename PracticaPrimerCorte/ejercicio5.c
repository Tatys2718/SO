#include <stdio.h>
#include <stdlib.h>

int** convertirMatrix(int **matrix, int **matrix_trans, int fila, int col);

int main (int argc, char **argv){
    int fila;
    int fila_tran;
    int col;
    int col_tran;
    int **matrix;
    int **matrix_transpuesta;
    FILE *f = fopen(argv[1], "r");
    if(f == NULL){
        perror("Error al leer el archivo");
        return 1;
    }
    fscanf(f, "%d", &fila);
    fscanf(f, "%d", &col);

    fila_tran = col;
    col_tran = fila;

    matrix = (int **) malloc (fila * sizeof(int *));
    if(!matrix){
        printf("Error en malloc 1. \n");
        return 1;
    }

    for(int i = 0; i < fila; i++){
        matrix [i]= (int *) malloc (col* sizeof(int));
        if(!matrix[i]){
            printf("Error en malloc 2 \n");
        }
    }

    matrix_transpuesta = (int **) malloc (fila_tran * sizeof(int *));
    if(!matrix_transpuesta){
        printf("Error en malloc 1. \n");
        return 1;
    }

    for(int i = 0; i < fila; i++){
        matrix_transpuesta [i]= (int *) malloc (col_tran* sizeof(int));
        if(!matrix_transpuesta[i]){
            printf("Error en malloc 2 \n");
        }
    }

    for(int i = 0; i < fila; i++){
        for(int j = 0; j < col; j++){
            fscanf(f, "%d", &matrix[i][j]);
        }
    }

    matrix_transpuesta = convertirMatrix(matrix, matrix_transpuesta, fila, col);

    for(int i = 0; i < fila_tran ; i++){
        for(int j = 0; j < col_tran; j++ ){
            printf("%d ", matrix_transpuesta[i][j]);
        }
        printf("\n");
    }
    
    for(int i = 0; i < fila; i++){
        free(matrix[i]);
    }
    free(matrix);
    fclose(f);
    return 0;
}
int** convertirMatrix(int **matrix, int **matrix_trans, int fila, int col){
    for(int i = 0; i < fila; i++){
        for(int j= 0 ; j< col; j++){
            matrix_trans[j][i] =  matrix[i][j];
        }
    }
    return matrix_trans;
}