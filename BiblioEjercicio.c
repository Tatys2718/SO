#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 256

typedef struct libro{
    char name[MAX_LEN];
    int year;
    int n_pages;
}libro;

void read_file (const char *file, char ***vector, int *tam);
//void read_file_sede(char* sede);

int main (int argc, char **argv){
    if(argc < 2){
        perror("Se necesita un archivo \n");
        return EXIT_FAILURE;
    }
    pid_t root = getpid();
    int id_child;

    char **vector;
    int tam;
    int chunk;



    read_file(argv[1], &vector, &tam);
    printf("La cantidad de sedes es: %d \n", tam);
    int n_child = tam;
    chunk = tam / n_child;

    for(id_child = 0; id_child < n_child; id_child ++){
        if(!fork()){
            break;
        }
    }
    if(root == getpid()){ //PADRE
        for(int i = 0; i < n_child; i++){
            wait(NULL);
        }
    }else{ //HIJO
        int indx_inicio, indx_fin;
        indx_inicio = chunk * id_child;
        if(id_child == n_child-1){
            indx_fin = tam;
        }else{
            indx_fin = chunk * (id_child + 1);
        }
        FILE *fl = fopen(vector[id_child], "r");
        if(fl == NULL){
            perror("Error al abrir el archivo del proceso");
            exit(-1);
        }
        int size;
        libro *lib;
        fscanf(fl, "%d\n", &size);
        lib = (libro*) malloc(size * sizeof(libro));
        for(int i = 0; i < size; i++){
            fscanf(fl, "%[^,], %d, %d\n", lib[i].name, &lib[i].year, &lib[i].n_pages);
        }
        libro aux;
        for(int i = 0; i<size-1; i++){
            for(int j = 0; j<size-i-1; j++){
                if(lib[j].year > lib[j+1].year){
                    aux = lib[j];
                    lib[j] = lib[j+1];
                    lib[j+1] = aux;
                }
            }
        }
        for(int i = 0; i<size; i++){
            printf("Sede: %s\n Nombre: %s, Año: %d, N_pag: %d \n", vector[id_child], lib[i].name, lib[i].year, lib[i].n_pages);
        }
        
        free(lib);
        fclose(fl);
    }

    free(vector);
    vector = NULL;

    return EXIT_SUCCESS;
    
}
void read_file (const char *file, char ***vector, int *tam){
    FILE *f = fopen(file, "r");

    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }

    char buffer[MAX_LEN];

    fscanf(f, "%d\n", tam);

    *vector = (char**) malloc(*tam * sizeof(char *));

    if(!(*vector)){
        perror("Fallo malloc \n");
        exit(-1);
    }

    for(int i = 0; i < *tam; i++){
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        (*vector)[i] = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy((*vector)[i], buffer);
    }
    for(int i = 0; i < *tam; i++){
        printf("%s ---- y longitud %ld\n", (*vector)[i], strlen((*vector)[i]));
    }

    fclose(f);

}

/*void read_file_sede(char* sede){
    FILE *fs = fopen(sede, "r");

    fclose(fs);
}
    */