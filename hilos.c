//Taller letras en diagonal
#include <stdio.h>   
#include <unistd.h>     
#include <stdlib.h>     
#include <pthread.h>
#include <string.h>
#define MAX_LENGTH 256

void *buscar_arriba_abajo(void* arg);
void *buscar_abajo_arriba(void* arg);

struct  contenido{
    int rows;
    int cols;
    int n_words;
};


char **matrix;
char **vector;
char buffer[MAX_LENGTH];

int main (int argc, char ** argv){
    if(argc < 2){
        perror("Se debe enviar el archivo porfavor.\n");
    }
    pthread_t hilo_arriba_abajo;
    pthread_t hilo_abajo_arriba;
    struct contenido dato;
    
    FILE *f = fopen(argv[1], "r");

    if(f == NULL){
        perror("Error al leer el archivo");
        return 1;
    }

    fscanf(f, "%d", &dato.rows);
    fscanf(f, "%d", &dato.cols);

    printf("Filas: %d \n", dato.rows);
    printf("Columnas: %d \n", dato.cols);

    matrix = (char **) malloc (dato.rows * sizeof(char *));
    if(!matrix){
        printf("Error en malloc 1. \n");
        return 1;
    }

    for(int i = 0; i <= dato.rows; i++){
        matrix [i]= (char *) malloc (dato.cols* sizeof(char));
        if(!matrix[i]){
            printf("Error en malloc 2 \n");
        }
    }

    for(int i = 0; i <= dato.rows; i++){
        for(int j = 0; j < dato.cols; j++){
            fscanf(f, "%c", &matrix[i][j]);
        }
    }

    for(int i = 0; i <= dato.rows; i++){
        for(int j = 0; j < dato.cols; j++){
            printf("%c", matrix[i][j]);
        }
        
    }
    printf("\n");

    fscanf(f, "%d", &dato.n_words);
    printf("La cantidad de palabras a buscar es: %d \n", dato.n_words);


    vector = (char **) malloc (dato.n_words * sizeof(char *));
    if(!vector){
        printf("Error en malloc 1 vector. \n");
        return 1;
    }

    for(int i = 0; i < dato.n_words; i++){
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        vector [i]= (char *) malloc (strlen(buffer + 1)* sizeof(char));
        strcpy(vector[i], buffer);
        if(!vector[i]){
            printf("Error en malloc 2 \n");
        }
    }
    
    if(pthread_create(&hilo_arriba_abajo, NULL, buscar_arriba_abajo, (void*)&dato) != 0){
        perror("Error al crear el hilo 1");
        return 1;
    }
    if(pthread_create(&hilo_abajo_arriba, NULL, buscar_abajo_arriba, (void*)&dato) != 0){
        perror("Error al crear el hilo 2");
        return 1;
    }
    

    pthread_join(hilo_abajo_arriba, NULL);
    pthread_join(hilo_arriba_abajo, NULL);

    for (int i = 0; i < dato.rows ; i++){
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; i < dato.n_words ; i++){
        free(vector[i]);
    }
    free(vector);

    fclose(f);
    return 0;
}
void *buscar_arriba_abajo(void* arg){
    struct contenido* p = (struct contenido *)arg;
    for(int k = 0; k < p->n_words; k++){
        printf("Palabra a buscar es: %s\n", vector[k]);
        int hallada = 0;
        char *palabra = vector[k];
        int longitud = strlen(palabra);
        for(int i = 0; i < p->rows; i++){
            for(int j = 0; j < p->cols; j++){
                if(i + longitud <= p->rows && j + longitud <= p->cols){
                    int encontrada = 1;
                    for(int h = 0; h < longitud; h++){
                        if(matrix[i+h][j+h] != palabra[h]){
                            encontrada = 0;
                            break;
                        }
                        if(matrix[i+h][j-h] != palabra[h]){
                            encontrada = 0;
                            break;
                        }
                    }
                    if(encontrada){
                        printf("Hilo arriba-abajo encontró %s en %d,%d\n", palabra, i, j);
                        hallada = 1;
                        break;
                    }
                }
            }
            if(hallada){
                break;
            }

        }
        //if(!hallada){
        //   printf("%s no encontrada\n", palabra);
        //}
    }
    

}
void *buscar_abajo_arriba(void* arg){
    struct contenido* p = (struct contenido *)arg;
    for(int k = 0; k < p->n_words; k++){
        printf("Palabra a buscar es: %s\n", vector[k]);
        int hallada = 0;
        char *palabra = vector[k];
        int longitud = strlen(palabra);
        for(int i = 0; i < p->rows; i++){
            for(int j = 0; j < p->cols; j++){
                if(i + longitud <= p->rows && j + longitud <= p->cols){
                    int encontrada = 1;
                    for(int h = 0; h < longitud; h++){
                        if(matrix[i-h][j+h] != palabra[h]){
                            encontrada = 0;
                            break;
                        }
                        else if(matrix[i-h][j-h] != palabra[h]){
                            encontrada = 0;
                            break;
                        }
                    }
                    if(encontrada){
                        printf("Hilo abajo_arriba encontró %s en %d,%d\n", palabra, i, j);
                        hallada = 1;
                        break;
                    }
                } 
            }
            if(hallada){
                break;
            }

        }
        //if(!hallada){
        //   printf("%s no encontrada\n", palabra);
        //}
    }
    
}