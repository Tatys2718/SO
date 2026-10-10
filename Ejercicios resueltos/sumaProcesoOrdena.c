#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <time.h>

void read_file(const char *file, int **vector, int *tam);
void quicksort(int *vector, int start, int end);
int partition (int *vector, int start, int end);

int main (int argc, char **argv){
    if(argc < 3){
        perror("Se necesita un archivo y los hijos\n");
        return EXIT_FAILURE;
    }
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME,&start);
    
    pid_t root = getpid();
    int *vector;
    int tam; 
    int child_id;

    int n_children = atoi(argv[2]);
    
    int chunk;

    remove("out.txt");

    read_file(argv[1], &vector, &tam);

    chunk = tam/n_children;
    printf("Chunk: %d \n", chunk);

    for(child_id = 0; child_id < n_children; child_id ++){
        if(!fork()){
            break;
        }
    }

    FILE *fi = fopen("out.txt", "a"); 

    if(root == getpid()){//Soy padre
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
        long int suma1 = 0;
        long int suma2 = 0;
        long int total = 0;
        fi = fopen("out.txt", "r");
        if(fi == NULL){
            perror("Error al abrir el archivo. \n");
            exit(-1);
        }
        fscanf(fi, "%ld", &suma1);
        fscanf(fi, "%ld", &suma2);
        total = suma1 + suma2;
        printf("La suma total es: %ld\n", total);

        quicksort(vector, 0 , tam-1);
        
        int cont1 = 0;
        int cont2 = 0;
        int numrep;
        for(int i = 0; i < tam ; i++){
            if(vector[i+1] == vector[i] ){
                cont1 = cont1 + 1;
            }else{
                if(cont1 > cont2){
                    cont2 = cont1;
                    numrep = vector[i];
                    printf("El numero que más se repite es: %d con frecuencia %d\n", numrep, cont2);
                }
                cont2 = 0;
                cont1 = 0;
            }
        }
        /*
        for(int i = 0; i<tam; i++){
            printf("%d ", vector[i]);
        }*/

        clock_gettime(CLOCK_REALTIME, &end);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Tiempo estimado: %f\n", elapsed);

    }else{//Soy hijo wiiiiii
        int indx_inicial, indx_final;
        indx_inicial = chunk * child_id;
        if(child_id == n_children-1){
            indx_final = tam;
        }else{
            indx_final = chunk * (child_id + 1);
        }
        long int suma = 0;
        for(int i = indx_inicial; i < indx_final; i++){
            suma = suma + vector[i];
        }
        fprintf(fi, "%ld\n", suma);
    }
    fclose(fi);

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
    printf("El tamaño del archivo es: %d\n", *tam);

    *vector = (int*) malloc (*tam * sizeof(int));

    for(int i = 0; i < *tam; i ++){
        if(fscanf(f, "%d", &(*vector)[i]) != 1){
            fprintf(stderr, "No se pudo leer el archivo en la posición: %d\n", i);
            EXIT_FAILURE;
        }
    } 
    
    fclose(f);
    
}

void quicksort(int *vector, int start, int end){
    if(end <= start){
        return;
    }
    int pivot = partition (vector, start, end);
    quicksort(vector, start, pivot - 1);
    quicksort(vector, pivot+1, end);

}
int partition (int *vector, int start, int end){
    int pivot = vector[end];
    int i = start-1;
    for(int j = start; j<= end -1; j++){
        if(vector[j]<pivot){
            i++;
            int temp = vector[i];
            vector[i] = vector[j];
            vector[j] = temp;
        }
    }
    i++;
    int temp = vector[i];
    vector[i] = vector[end];
    vector[end] = temp;
    return i;
}