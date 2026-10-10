#include <stdio.h>
#include <stdlib.h>

void print_matrix (char **matrix, int fila, int col);

int main (int argc, char **argv){
    int fila_int;
    int col_int;
    char **matrix;
    int fila = atoi(argv[1]);
    int col = atoi (argv[2]);

    printf("%d\n", fila);
    printf("%d\n", col);

    fila_int = fila - 2;
    col_int = col - 2;

    matrix = (char **) malloc (fila * sizeof(char *));
    if(!matrix){
        printf("Error en malloc 1.\n");
        return 1;
    }
    for (int i = 0 ; i < fila; i++){
        matrix[i] = (char *) malloc (col * sizeof(char));
        if(!matrix[i]){
            printf("Error en malloc 2 \n");
        }
    }
    for(int i = 0; i < fila ; i++){
        for(int j = 0;j < col; j ++){
            if(i == 0 || j == 0 || i == fila-1 || j == col-1){
                matrix[i][j] = 'C';
            }else{
                matrix[i][j] = '.';
            } 
        }

    }

    print_matrix(matrix, fila, col);

    for (int i = 0; i < fila ; i++){
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}

void print_matrix (char **matrix, int fila, int col){
    for (int i = 0; i < fila ; i++){
        for(int j = 0; j < col; j++){
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}