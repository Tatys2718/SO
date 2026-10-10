#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>
#include <time.h>

void showtree();
unsigned int sizeof_dm(int rows, int cols, size_t size_elements);
void create_index(void **matrix, int rows, int cols, size_t size_elements);
void create_shm(int *shm_id, void **mtx, int rows, int cols, size_t size_elements);
void read_file_Dim(const char *file, int *rows, int *cols, int *dias);
void read_file (const char *file, int rows, int cols, int **matrix);
void signal_handler(int sig);

volatile sig_atomic_t flag = 0;
int main (int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo porfavor\n");
        EXIT_FAILURE;
    }
    pid_t root = getpid();
    int n_children = 4;
    int child_id;
    pid_t children[n_children];
    int **matrix;
    int **resultante;
    int shm_id;
    int shm_id_result;
    int dias;
    int rows;
    int cols;
    signal(SIGUSR1, signal_handler);

    read_file_Dim(argv[1], &rows, &cols, &dias);
    
    create_shm(&shm_id, (void **)&matrix, rows, cols, sizeof(int));
    create_shm(&shm_id_result, (void **)&resultante, rows, cols, sizeof(int));
    read_file(argv[1], rows, cols, matrix);

    if(matrix == NULL){
        perror("fallo shmat");
        exit(1);
    }
    if(resultante == NULL){
        perror("Fallo shmat 2");
        exit(1);
    }

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!(children[child_id])){
            break;
        }
    }

    if(root == getpid()){
        showtree();
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        kill(children[0], SIGUSR1);
        while(!flag);

        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }

        printf("\n");

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                printf("%d ", resultante[i][j]);
            }
            printf("\n");
        }

    }else{
        srand(getpid());
        for(int d = 1; d <= dias; d++){

            while(!flag);
            flag = 0;

            int di[8] = {-1,-1,-1, 0,0, 1,1,1};
            int dj[8] = {-1, 0, 1,-1,1,-1,0,1};
            if(child_id == 0){
                printf("Soy el hijo (%d) y recibi la señal. \n", getpid());
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        resultante[i][j] = matrix[i][j];
                        if(matrix[i][j] == 0){ //Si su nivel de contaminación es bajo
                            printf("Entre a la posicion: (%d,%d)\n", i, j);
                            int count_vec = 0;
                            for(int k = 0; k < 8; k++){
                                int ni = i + di[k];
                                int nj = j + dj[k];

                                if(ni >= 0 && ni < rows && nj >= 0 && nj < cols){
                                    if(matrix[ni][nj] == 1 || matrix[ni][nj] == 2){
                                        count_vec = count_vec + 1;
                                    }
                                }
                                printf("La posicion: (%d,%d) tiene %d vecinos medio o alta\n", i, j, count_vec);
                            }
                            if(count_vec >= 4){ //tiene al menos 4 vecinos con nivel de contaminación alto o medio
                                resultante[i][j] = 1; //su nivel de contaminación pasa a ser medio
                            }
                        }
                    }
                }
                printf("Dia: %d\n", d);
                kill(children[1], SIGUSR1);
            }else if(child_id == 1){
                double probabilidad;
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        resultante[i][j] = matrix[i][j];
                        if(matrix[i][j] == 1){//Si su nivel de contaminacion es medio
                            int count_vec1 = 0;
                            for(int k = 0; k < 8; k++){
                                int ni = i + di[k];
                                int nj = j + dj[k];
                                if(ni >= 0 && ni < rows && nj >= 0 && nj < cols){
                                    if(matrix[ni][nj] == 0){
                                        count_vec1 = count_vec1 + 1;
                                    }
                                }
                            }
                            probabilidad = 0.1 + (count_vec1 * 0.03);
                            double random = (double)rand() / RAND_MAX;
                            if(random < probabilidad){
                                resultante[i][j] = 0;
                            }
                        }
                    }
                }
                printf("Hijo 2: Dia: %d\n", d);
                kill(children[2], SIGUSR1);
            }else if(child_id == 2){
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        resultante[i][j] = matrix[i][j];
                        if(matrix[i][j] == 1){  //Contaminación media
                            int count_vec2 = 0;
                            for(int k = 0; k < 8; k++){
                                int ni = i + di[k];
                                int nj = j + dj[k];
                                if(ni >= 0 && ni < rows && nj >= 0 && nj < cols){
                                    if(matrix[ni][nj] == 2){
                                        count_vec2 = count_vec2 + 1;
                                    }
                                }
                            }
                            if(count_vec2 >= 3){
                                resultante[i][j] = 2;
                            }
                        }
                    }
                }
                printf("Hijo 3: Dia: %d\n", d);
                kill(children[3], SIGUSR1);
            }else{
                double probabilidad2;
                for(int i = 0;i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        resultante[i][j] = matrix[i][j];
                        if(matrix[i][j] == 2){
                            int count_vec3 = 0;
                            for(int k = 0; k < 8; k++){
                                int ni = i + di[k];
                                int nj = j + dj[k];
                                if(ni >= 0 && ni < rows && nj >= 0 && nj < cols){
                                    if(matrix[ni][nj] == 0){
                                        count_vec3 = count_vec3 + 1;
                                    }
                                }
                            }
                            probabilidad2 = 0.15 + (count_vec3 * 0.05);
                            double random1 = (double)rand() / RAND_MAX;
                            if(random1 < probabilidad2){
                                resultante[i][j] = 1;
                            }
                        }
                    }
                }
                //copy matrix resultante a matrix normal
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        matrix[i][j] = resultante[i][j];
                    }
                }
                printf("Hijo 4: Dia: %d\n", d);
                if(d <= dias-1){
                    kill(children[0], SIGUSR1);
                }else{
                    kill(getppid(), SIGUSR1);
                }
            }
        }
    }
    shmdt(matrix);
    shmctl(shm_id, IPC_RMID, NULL);
    shmdt(resultante);
    shmctl(shm_id_result, IPC_RMID, NULL);
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

void read_file_Dim(const char *file, int *rows, int *cols, int *dias){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    fscanf(f, "%d\n", dias);
    fscanf(f, "%d\n", rows);
    fscanf(f, "%d\n", cols);

    fclose(f);
}

void read_file (const char *file, int rows, int cols, int **matrix){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
    }
    int salto;
    fscanf(f, "%d", &salto);
    fscanf(f, "%d", &salto);
    fscanf(f, "%d", &salto);

    for(int i = 0; i < rows; i++){ 
        for (int j = 0; j < cols; j++) { 
            fscanf(f, "%d", &((matrix)[i][j])); 
        } 
    }
    fclose(f);
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}

void signal_handler(int sig){
    flag = 1;
}