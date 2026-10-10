#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>

void showtree();
unsigned int sizeof_dm (int rows, int cols, size_t sizeElements);
void create_index(void **m, int rows, int cols, size_t sizeElement);
void create_shm(int *shm_id, void **mtx, int rows, int cols, size_t size_elements);
void read_file_Dim(const char *file, int *rows, int *cols);
void read_file (const char *file, int rows, int cols, int **matrix);
int main (int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo porfavor\n");
        return EXIT_SUCCESS;
    }

    pid_t root = getpid();
    int **matrix;
    int **mtx_resultados;
    int rows, cols;//N = rows, B = cols+1
    int shm_id;
    int result_shm_id;
    size_t size_matrix;
    int n_children = 3;
    int child_id;
    int count_bloque = 0;
    int count_vid_sos = 0;

    read_file_Dim(argv[1], &rows, &cols);
    cols = cols + 1;
    printf("Las filas son: %d y las columnas son: %d \n", rows, cols);

    create_shm(&shm_id, (void **)&matrix, rows, cols, sizeof(int));
    create_shm(&result_shm_id, (void **)&mtx_resultados, rows, cols, sizeof(int));

    if(matrix == NULL){
        perror("fallo shmat");
        exit(1);
    }

    if(mtx_resultados == NULL){
        perror("Fallo shmat 2");
        exit(1);
    }

    for(child_id = 0; child_id < n_children; child_id++){
        if(!fork()){
            break;
        }
    }

    if(root == getpid()){ //I am daddy
        showtree();
        read_file(argv[1], rows, cols, matrix);
        /*
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                mtx_resultados[i][j] = 0;
            }
        }*/
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(mtx_resultados[i][j] > 0){
                    count_bloque = count_bloque + 1;
                }
            }
            if(count_bloque >= 2){
                count_vid_sos = count_vid_sos + 1;
            }
            count_bloque = 0;
            printf("\n");
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                printf("%d ", mtx_resultados[i][j]);
            }
            printf("\n");
        }
        printf("Cantidad de videos sospechosos: %d \n", count_vid_sos);

        shmdt(matrix);
        shmctl(shm_id, IPC_RMID, NULL);
        shmdt(mtx_resultados);
        shmctl(result_shm_id, IPC_RMID, NULL);
    }else{ //I am children
        sleep(1);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(child_id == 0){
                    if(j == 2){
                        if(matrix[i][j] >= 80){
                            mtx_resultados[i][j] = 1;
                        }
                    }
                }else if(child_id == 1){
                    if ( j == 3){
                        if(matrix[i][j] >= 75){
                            mtx_resultados[i][j] = 1;
                        }
                    }
                }else{
                    if(j == 4){
                        if(matrix[i][j] >= 35){
                            mtx_resultados[i][j] = 1;
                        }
                    }
                }
            }
        }
        shmdt(matrix);
        shmdt(mtx_resultados);
    }
    return EXIT_SUCCESS;
}
unsigned int sizeof_dm (int rows, int cols, size_t sizeElement){
    size_t size;
    size = rows * sizeof(void*);
    size = size + (cols * rows * sizeElement);
    return size;
}
void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for(i = 1; i < rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}
void create_shm(int *shm_id, void **mtx, int rows, int cols, size_t size_elements){
    size_t size_mtx;

    size_mtx = sizeof_dm(rows, cols, sizeof(size_elements));
    *shm_id = shmget(IPC_PRIVATE, size_mtx, 0666 | IPC_CREAT);
    if(*shm_id == -1){
        perror("Fail shmget");
        exit(1);
    }

    *mtx = shmat(*shm_id, NULL, 0);
    if(*mtx == NULL){
        perror("Fail Shmat");
        exit(1);
    }

    create_index((void*)*mtx, rows, cols, size_elements);
}

void read_file (const char *file, int rows, int cols, int **matrix){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
    }
    int salto;
    fscanf(f, "%d", &salto);
    fscanf(f, "%d", &salto);

    for(int i = 0; i < rows; i++){ 
        for (int j = 0; j < cols; j++) { 
            fscanf(f, "%d", &((matrix)[i][j])); 
        } 
    }
    fclose(f);
}

void read_file_Dim(const char *file, int *rows, int *cols){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    fscanf(f, "%d", cols);
    fscanf(f, "%d", rows);

    fclose(f);
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}