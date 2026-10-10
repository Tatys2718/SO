#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>

void showtree();
unsigned int sizeof_dm(int rows, int cols, size_t size_elements);
void create_index(void **matrix, int rows, int cols, size_t size_elements);
void create_shm(int *shm_id, void **mtx, int rows, int cols, size_t size_elements);
void read_file (const char *file, int rows, int cols, int **matrix);

typedef struct {
    int transaction_id;
    int user_id;
    int amount;
    int trans_last_hour;
    int country_risk;
    int failed_attemps;
    int device_type;

    int amount_risk;
    int trans_risk;
    int count_risk;
    int failed_risk;
    int count_suspisous;
    int is_suspisous; //0 = no, 1 = si es sospechoso
} Result;

typedef struct {
    int done[3];
    int end;
    int block_num;
} SharedData;

int main (int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo\n");
        EXIT_FAILURE;
    }

    pid_t root = getpid();
    int B;
    int N;
    int cols = 7;
    int **bloque;
    int n_children = 4;
    int shm_id;
    int share_shm_id;
    int results_shm_id;
    int child_id;

    SharedData *shared;
    Result *results;

    FILE *f = fopen(argv[1], "r");

    if(f == NULL){
        perror("Error al abrir el archivo");
        return 1;
    }

    fscanf(f, "%d", &B);
    fscanf(f, "%d", &N);

    fclose(f);

    create_shm(&shm_id, (void **)&bloque, B, cols, sizeof(int));
    share_shm_id = shmget(IPC_PRIVATE, sizeof(SharedData), 0666 | IPC_CREAT);
    results_shm_id = shmget(IPC_PRIVATE, MAX_USERS * sizeof(Result), 0666 | IPC_CREAT);

    shared = (SharedData*) shmat(shared_shm_id, NULL, 0);
    results = (Result*) shmat(results_shm_id, NULL, 0);

    memset(shared, 0, sizeof(SharedData));
    memset(results, 0, sizeof(Result));


    if(bloque == NULL){
        perror("fallo shmat");
        exit(1);
    }

    for(child_id = 0; child_id < n_children; child_id++){
        if(!fork()){
            break;
        }
    }

    if(root == getpid()){
        showtree();
        int total_fraudulentos, total_sospechosos;

        shared->end = 0;
        shared->block_num = 0;
        total_fraudulentos = total_sospechosos = 0;

        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }

        for(int i = 0; i < B; i++){
            for(int j = 0; j < cols; j++){
                printf("%d ", bloque[i][j]);
            }
            printf("\n");
        }

        shared->done[0] = 0;
        shared->done[1] = 0;
        shared->done[2] = 0;

        shared->block_num++;
        while(!shared->done[0] || !shared->done[1] || !shared->done[2]){
            usleep(5000);
        }

        for(int j=0; j < B; j++){
            if((results[bloque[j][1]].amount_risk + 
                results[bloque[j][1]].count_risk + 
                results[bloque[j][1]].failed_risk + results [bloque[j][1]].trans_risk) >= 2){
                    
                results[bloque[j][1]].count_suspisous++;
                results[bloque[j][1]].is_suspisous = 1;
                total_sospechosos++;
            }
        }

        

        for(int i=0; i < n_children; i++) wait(NULL);

    }else{
        if(child_id == 0){
            int salto;
            FILE *f = fopen(argv[1], "r");

            if(f == NULL){
                perror("Error al abrir el archivo");
                return 1;
            }

            fscanf(f, "%d", &salto);
            fscanf(f, "%d", &salto);
            
            for(int i=0; i < N/B; i++){
                for(int j=0; j < B; j++){
                    fscanf(fl, "%d %d %d %d %d %d", &bloque[j][0],
                        &bloque[j][1], &bloque[j][2], &bloque[j][3],
                        &bloque[j][4], &bloque[j][5], &bloque[j][6]
                    );
                }
            }

            fclose(f);

        }else if(child_id == 1){
            int count_monto;
            for(int i = 0; i < B; i++){
                for (int j = 0; j < cols; j++){
                    if(j == 2){
                        if(bloque[i][j] >= 10000){
                            count_monto = count_monto + 1;
                        }
                    }
                }
            }
        }else if(child_id == 2){
            sleep(1);
        }else{
            sleep(1);
        }
    }

    shmdt(bloque);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
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

unsigned int sizeof_dm(int rows, int cols, size_t size_elements){
    size_t size = rows * sizeof(void *);
    size += (rows * cols * size_elements); 
    return size;
}

void create_index(void **matrix, int rows, int cols, size_t size_elements){
    int i;
    size_t size_rows = cols * size_elements;
    matrix[0] = matrix + rows;
    for(i = 1; i < rows; i++){
        matrix[i] = (matrix[i-1] + size_rows);
    }
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}