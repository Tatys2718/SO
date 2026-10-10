#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#define MAX_LEN 256

void showtree();
void read_file(const char *file, char ***vector, int *tam);
int main(int argc, char **argv){
    if(argc < 2){
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }
    pid_t root = getpid();
    int n_child = 3;
    int child_id;

    int tam;
    int chunk;
    char **vector;
    char **vectorModelo;
    int tam2;

    read_file(argv[1], &vector, &tam);

    chunk = tam/n_child;

    for(child_id = 0; child_id < n_child; child_id ++){
        if(!fork()){
            break;
        }
    }

    if(root == getpid()){ //Soy PADRE
        /*for(int i = 0; i < tam; i++){
            printf("Archivo: %s \n", vector[i]);
        }*/
        showtree();
        for(int i = 0; i < n_child; i++){
            wait(NULL);
        }
        for(int i = 0; i<tam; i++){
            free(vector[i]);
        }
        free(vector);
        vector = NULL;
        
    }else{ //Soy Hijo
        int indx_inicio, indx_fin;
        printf("[%d] -> [CHILD_ID] = %d\n", getpid(), child_id);
        indx_inicio = chunk * child_id;
        if(child_id == n_child-1){
            indx_fin = tam;
        }else{
            indx_fin = chunk * (child_id + 1);
        }
        
        for(int i = indx_inicio; i < indx_fin; i++){
            read_file(vector[i], &vectorModelo, &tam2);
            printf("El hijo [%d] leyo el archivo: %s \n", child_id, vector[i]);
        }
        for(int i = 0; i<tam2; i++){
            free(vectorModelo[i]);
        }
        free(vectorModelo);
        vectorModelo = NULL;
    }
    
    

    return EXIT_SUCCESS;
}

void read_file(const char *file, char ***vector, int *tam){
    FILE *f = fopen(file, "r");
    if (f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }

    char buffer[MAX_LEN];
    fscanf(f, "%d\n", tam);

    *vector = (char **) malloc(*tam * sizeof(char*));
    if(!(*vector)){
        perror("Fallo malloc \n");
        exit(-1);
    }
    for(int i = 0;i < *tam; i++){
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        (*vector)[i] = (char *)malloc ((strlen(buffer) + 1) * sizeof (char));
        strcpy ((*vector)[i], buffer);
    }
    for(int i = 0; i < *tam; i++){
        printf("%s ---- y longitud %ld\n", (*vector)[i], strlen((*vector)[i]));
    }

    fclose(f);
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}