#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_LEN 100

void read_file(const char *file, int *rows, int cols, int ***matrix, int *lote);
void showtree();
int main (int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo");
    }
    pid_t root = getpid();
    int **matrix;
    int rows;
    int cols = 6;
    int lote;
    int n_children = 3;
    int child_id;
    int fin; //bandera para saber cuando el hijo ya no recibira más lotes o bloques ya no se ni que hago
    int n_pipes = n_children*2;
    int pipes[n_pipes][2];

    for(int i = 0; i < n_pipes; i++){
        if(pipe(pipes[i]) == -1){
            perror("Fallo la tuberia\n");
            EXIT_FAILURE;
        }
    }

    for(child_id = 0; child_id < n_children; child_id ++){
        if(!fork()){
            break;
        }
    }
    if(root == getpid()){
        showtree();
        read_file(argv[1], &rows, cols, &matrix, &lote);

        printf("El valor de cols = %d y el valor de N = %d \n", cols, rows);
        
        for(int i = 0; i < rows ; i++){
            for(int j = 0; j < cols; j++ ){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        for(int i = 0; i < n_pipes; i++){
            if(i % 2 == 0){
                close(pipes[i][0]);
            }else{
                close(pipes[i][1]);
            }
        }
        for(int i = 0; i < n_pipes; i++){
            if(i % 2 == 0){
                write(pipes[i][1], &lote, sizeof(int));
            }
        }
        int count;
        for(int k = 0; k < rows; k+=lote){
            count = 0;
            for(int i = k; i < k+lote; i++){
                for(int j = cols - 1; j >= 0; j--){
                    //printf("Padre: Estoy enviando %d a la tuberia %d \n", matrix[i][j], count);
                    write(pipes[count][1], &matrix[i][j], sizeof(int));
                }
                count = count + 2;
            }
        }
        close(pipes[0][1]);
        close(pipes[2][1]);
        close(pipes[4][1]);
        
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }

        int count1;
        int video_sos_1 = 0;
        int count2;
        int video_sos_2 = 0;
        int count3;
        int video_sos_3 = 0;
        while((read(pipes[1][0], &count1, sizeof(count1))) > 0){
            video_sos_1 = video_sos_1 + count1;
        }
        printf("PADRE: Recibi que la cantidad de videos sospechosos del hijo 1 es: %d \n", video_sos_1);
        close(pipes[1][0]);
        while((read(pipes[3][0], &count2, sizeof(int))) > 0){
            video_sos_2 = video_sos_2 + count2;
        }
        printf("PADRE: Recibi que la cantidad de videos sospechosos del hijo 2 es: %d \n", video_sos_2);
        close(pipes[3][0]);
        while((read(pipes[5][0], &count3, sizeof(int))) > 0){
            video_sos_3 = video_sos_3 + count3;
        }
        printf("PADRE: Recibi que la cantidad de videos sospechosos del hijo 3 es: %d \n", video_sos_3);
        close(pipes[5][0]);

        int videos_sospechosos = video_sos_1 + video_sos_2 + video_sos_3;

        printf("La cantidad de videos sospechosos es: %d \n", videos_sospechosos);

        for (int i = 0; i < rows ; i++){
            free(matrix[i]);
        }
        free(matrix);
    }else{
        int buffer;
        for(int i = 0; i < n_pipes; i++){
            if(child_id == 0){
                if(i == 0){
                    close(pipes[i][1]);
                }else if(i == 1){
                    close(pipes[i][0]);
                }else{
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
            }else if(child_id == 1){
                if(i == 2){
                    close(pipes[i][1]);
                }else if(i == 3){
                    close(pipes[i][0]);
                }
                else{
                    close(pipes[i][0]);
                    close(pipes[i][i]);
                }

            }else{
                if(i == 4){
                    close(pipes[i][1]);
                }else if(i == 5){
                    close(pipes[i][0]);
                }else{
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
            }
        }
        if(child_id == 0){
            int usuarioB1[MAX_LEN];
            int count_sync = 0;
            int count_audio = 0;
            int count_face = 0;
            int count_video_sos1 = 0;
            int pos = 0;
            int lote;
            read(pipes[0][0],&lote, sizeof(lote));
            //printf("La cantidad de lotes que le llego al hijo 1 es: %d \n", lote);
            int aux = lote;
            int i = cols-1;
            while((read(pipes[0][0], &buffer, sizeof(buffer))) > 0){
                //printf("Soy el hijo 1 y me llego %d \n", buffer);
                if(i == 5){
                    i--;
                }
                else if(i == 4){
                    if(buffer <= 35){
                        count_sync = count_sync + 1;
                    }
                    i--;
                }
                else if(i == 3){
                   if(buffer >= 75){
                        count_audio = count_audio + 1;
                    }
                    i--;
                }
                else if(i == 2){
                    if(buffer >= 80){
                        count_face = count_face + 1;
                    }
                    i--;
                }
                else if( i == 1){
                    if((count_sync + count_audio + count_face) >= 2){
                        count_video_sos1 = count_video_sos1 + 1;
                        usuarioB1[pos] = buffer;
                        pos ++;
                    }
                    count_sync = 0;
                    count_audio = 0;
                    count_face = 0;
                    i--;
                }
                else if(i == 0){
                    aux = aux - 1;
                    //printf("Hijo 1 - lote %d\n", aux);
                    i = cols-1;
                }  
                if(aux == 0){
                    //printf("La cantidad de videos sospechosos del hijo 1 es %d\n", count_video_sos1);
                    write(pipes[1][1], &count_video_sos1 , sizeof(count_video_sos1));
                    aux = lote;
                    count_video_sos1 = 0;
                }
            }
            if(count_video_sos1 > 0){
                write(pipes[1][1], &count_video_sos1 , sizeof(count_video_sos1));
            }
            close(pipes[1][1]);
            close(pipes[0][0]);
        }else if(child_id == 1){
            int usuarioB2[MAX_LEN];
            int count_sync = 0;
            int count_audio = 0;
            int count_face = 0;
            int count_video_sos2 = 0;
            int pos = 0;
            int lote;
            read(pipes[2][0],&lote, sizeof(lote));
            printf("La cantidad de lotes que le llego al hijo 2 es: %d \n", lote);
            int aux = lote;
            int i = cols - 1;
            while((read(pipes[2][0], &buffer, sizeof(buffer))) > 0){  
                printf("Soy el hijo 2 y me llego %d  y mi columna es %d\n", buffer, i);
                if(i == 5){
                    i--;
                }else if(i == 4){
                    if(buffer <= 35){
                        count_sync = count_sync + 1;
                    }
                    i--;
                }else if(i == 3){
                    if(buffer >= 75){
                        count_audio = count_audio + 1;
                    }
                    i--;
                }else if(i == 2){
                    if(buffer >= 80){
                        count_face = count_face + 1;
                    }
                    i--;
                }else if( i == 1){
                    if((count_sync + count_audio + count_face) >= 2){
                        count_video_sos2 = count_video_sos2 + 1;
                        usuarioB2[pos] = buffer;
                        pos ++;
                    }
                    count_sync = 0;
                    count_audio = 0;
                    count_face = 0;
                    i--;
                }else if(i == 0){
                    aux = aux - 1;
                    i = cols - 1;
                }
                if(aux == 0){
                    printf("La cantidad de videos sospechosos del hijo 2 es %d\n", count_video_sos2);
                    write(pipes[3][1], &count_video_sos2 , sizeof(count_video_sos2));
                    aux = lote;
                    count_video_sos2 = 0;
                }
            }
            if(count_video_sos2 > 0){
                write(pipes[3][1], &count_video_sos2 , sizeof(count_video_sos2));
            }
            close(pipes[3][1]);
            close(pipes[2][0]);
        }else{
            int usuarioB3[MAX_LEN];
            int count_sync = 0;
            int count_audio = 0;
            int count_face = 0;
            int count_video_sos3 = 0;
            int pos = 0;
            int lote;
            read(pipes[4][0],&lote, sizeof(lote));
            //printf("La cantidad de lotes que le llego al hijo 3 es: %d \n", lote);
            int aux = lote;
            int i = cols - 1;
            while((read(pipes[4][0], &buffer, sizeof(buffer))) > 0){           
                //printf("Soy el hijo 3 y me llego %d \n", buffer);
                if(i == 5){
                    i--;
                }
                else if(i == 4){
                    if(buffer <= 35){
                        count_sync = count_sync + 1;
                    }
                    i--;
                }
                else if(i == 3){
                    if(buffer >= 75){
                        count_audio = count_audio + 1;
                    }
                    i--;
                }
                else if(i == 2){
                    if(buffer >= 80){
                        count_face = count_face + 1;
                    }
                    i--;
                }
                else if( i == 1){
                    if((count_sync + count_audio + count_face) >= 2){
                        count_video_sos3 = count_video_sos3 + 1;
                        usuarioB3[pos] = buffer;
                        pos ++;
                    }
                    i--;
                    count_sync = 0;
                    count_audio = 0;
                    count_face = 0;
                }
                else if(i == 0){
                    aux = aux - 1;
                    i = cols - 1;
                }
                if(aux == 0){
                    write(pipes[5][1], &count_video_sos3 , sizeof(count_video_sos3));
                    aux = lote;
                    count_video_sos3 = 0;
                }
            }
            if(count_video_sos3 > 0){
                write(pipes[5][1], &count_video_sos3 , sizeof(count_video_sos3));
            }
            close(pipes[5][1]);
            close(pipes[4][0]);
        }
    }
    return EXIT_SUCCESS;
}
void read_file(const char *file, int *rows, int cols, int ***matrix, int *lote){

    FILE *f = fopen(file, "r");

    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }


    fscanf(f, "%d", lote);
    fscanf(f, "%d", rows);

    *matrix = (int**) malloc(*rows * sizeof(int*));

    if(!(*matrix)){
        perror("Fallo malloc \n");
        exit(-1);
    }

    for(int i=0; i < *rows; i++){
        (*matrix)[i] = (int*) malloc(cols * sizeof(int));
        if(!(*matrix)[i]){
            perror("Fail malloc cols\n");
            exit(1);
        }
    }

    for(int i = 0; i < *rows; i++){ 
        for (int j = 0; j < cols; j++) { 
            fscanf(f, "%d", &(*matrix)[i][j]); 
        } 
    }

    printf("El valor de B = %d y el valor de N = %d \n", *lote, *rows);

    fclose(f);
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}