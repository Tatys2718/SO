#include <stdio.h>
#include <stdlib.h>

void normalizar (float **matriz, int fila, int col);
int max (float **matriz, int fila, int col);

int main (int argc, char **argv){
    int fila;
    int col;
    float **matrix;
    FILE *f = fopen(argv[1], "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
        return 1;
    }

    fscanf(f, "%d", &fila);
    fscanf(f, "%d", &col);

    printf("%d\n", fila);
    printf("%d\n", col);

    matrix = (float **) malloc(fila * sizeof(float *));
    if(!matrix){
        printf("Error: No se pudo malloc. \n");
        return 1;
    }

    for(int i = 0; i < fila; i++){
        matrix[i] = (float *) malloc (col * sizeof(float));
        if(!matrix[i]){
            printf("Error: no se puso malloc. 2\n");
        }
    }

    for(int i = 0; i < fila; i++){
        for(int j = 0; j < col; j++){
            fscanf(f, "%f", &matrix[i][j]);
        }
    }

    for(int i = 0; i < fila; i++){
        for(int j = 0; j < col; j++){
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }

    normalizar(matrix, fila, col);

    for(int i = 0; i < fila; i++){
        for(int j = 0; j < col; j++){
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
    
    for(int i = 0; i < fila; i++){
        free(matrix[i]);
    }
    free (matrix);
    fclose(f);
    return 0;
}
void normalizar (float **matriz, int fila, int col){
    int maxFila =-1;
    for(int i = 0; i < fila; i++){
        maxFila = max(matriz, i, col);
        for(int j = 0; j < col; j++){
            matriz[i][j] = matriz[i][j]/maxFila;
        }
    }
}

int max (float **matriz, int fila_actual, int col){
    int max = -1;
    for(int i = 0; i < col ; i++){
        if(max < matriz[fila_actual][i]){
            max = matriz[fila_actual][i];
        }
    }
    return max;
}
