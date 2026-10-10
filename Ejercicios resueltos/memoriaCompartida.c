#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

void showtree();
unsigned int sizeof_dm(int rows, int cols, size_t size_elements);
void create_index (void **matrix, int rows, int cols, size_t size_elements);
int main (int argc, char **argv){
    int **matrix;
    int rows, cols;
    rows = 3;
    cols = 3;
    size_t size_mtx;
    int x_shm;
    int shm_id;
    size_mtx = sizeof_dm(rows, cols, sizeof(int));
    shm_id = shmget(IPC_PRIVATE, size_mtx, IPC_CREAT | 0666);
    matrix = shmat(shm_id, NULL, 0);
    if(matrix == NULL){
        perror("fallo shmat");
        exit(1);
    }
    create_index((void**)matrix, rows, cols, sizeof(int));

    for(int i= 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            matrix[i][j] = 0;
        }
    }

    for(int i= 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    shmdt(matrix);
    shmctl(shm_id, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}
unsigned int sizeof_dm(int rows, int cols, size_t size_elements){
    size_t size = rows * sizeof(void*);
    size += (rows * cols * size_elements);
    return size; 
}
void create_index (void **matrix, int rows, int cols, size_t size_elements){
    int i;
    size_t size_rows = cols * size_elements;
    matrix[0] = matrix + rows;
    for(i = 1; i < rows; i++){
        matrix[i] = (matrix [i-1] + size_rows);
    }
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}