//Practica en el que el proceso hijo no pudo llamar a la función write
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

void showtree();
void read_file(const char *file, int **vector, int *tam);
void write_file(int inicio, int final, int id, int **vector);

int main (int argc, char **argv){
    if(argc < 2){
        perror("Se necesita archivo \n");
        return EXIT_FAILURE;
    }
    remove("out.txt");
    pid_t root = getpid();
    int n_child = 2;
    int child_id;

    int tam;
    int chunk;
    int *vector;

    read_file(argv[1], &vector, &tam);

    chunk = tam / n_child;
    printf("Chunk: %d\n", chunk);

    for(child_id = 0; child_id < n_child; child_id++){
        if(!fork()){
            break;
        }
    }

    if(root == getpid()){ //Proceso PADRE
        showtree();
        for(int i = 0; i < n_child; i++){
            wait(NULL);
        }
    }else{ //Proceso HIJO
        int indx_inicio, indx_final;
        printf("[%d] -> [CHILD_ID] = %d\n", getpid(), child_id);
        indx_inicio = chunk * child_id;
        if(child_id == n_child-1){
            indx_final = tam;
        }else{
            indx_final = chunk * (child_id + 1);
        }
        printf("[%d]-[IDX] = [%d][%d]\n\n", getpid(), indx_inicio, indx_final);
        printf("Good");
        write_file(indx_inicio, indx_final, child_id, &vector);
        printf("Good");

    }

    free(vector);

    return EXIT_SUCCESS;
}

void read_file(const char *file, int **vector, int *tam){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    
    fscanf(f, "%d", tam);
    printf("Size: %d\n", *tam);

    *vector = (int*) malloc(*tam * sizeof(int));
    if(!*vector){
        perror("Fallo malooc.\n");
        exit(-1);
    }

    for(int i = 0; i < *tam; i++){
        if(fscanf(f, "%d", &(*vector)[i]) != 1){
            fprintf(stderr, "Error no se pudo leer el elemento %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    /*for(int i = 0; i < *tam; i++){
        printf("%d ", (*vector)[i]);
    }*/

    printf("\n");

    fclose(f);
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}

void write_file(int inicio, int final, int id, int **vector){
    int sum1 = 0;
    int sum2 = 0;

    FILE *f = fopen("out.txt", "a");
    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    for(int i = inicio; i < final; i++){
        if(id == 0){
            sum1 = sum1 + *vector[i];
        }else{
            sum2 = sum2 + *vector[i];
        }
    }
    if(id == 0){
        fprintf(f, "%d", sum1);
    }else{
        fprintf(f, "%d", sum2);
    }
    fclose(f);
    printf("Good");
}