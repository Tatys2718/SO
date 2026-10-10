#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LEN 200

void showtree();
int main (int argc, char **argv){
    pid_t root = getpid();
    int n_children = 2;
    int n_tuberia = 8;
    int pipes[n_tuberia][2];
    char buffer[MAX_LEN];
    int child_id;
    int nivel = 2;
    int flag = 0;

    for(int i = 0; i < n_tuberia; i++){
        if(pipe(pipes[i]) == -1){
            perror("Error en la creación de la tuberia");
            EXIT_FAILURE;
        }
    }

    for(child_id = 0; child_id < n_children; child_id++){
        if(!fork()){
            if(!fork()){
                nivel = 2;
            }
            break;
        }
    }
    if(root == getpid()){
        showtree();
        char entrada [MAX_LEN];
        for(int j = 0; j < n_tuberia; j++){
            if(j == 0 || j == 2){
                close(pipes[j][1]);
            }else if(j == 1 || j == 3){
                close(pipes[j][0]);
            }else{
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
        }
        while(!flag){
            scanf ("%s", entrada);
            if(strcmp(entrada, "fin") == 0){
                flag = 1;
                break;
            }
            write(pipes[3][1], entrada, strlen(entrada)+1);
        }
        close(pipes[3][1]);

        while((read(pipes[2][0],buffer,sizeof(buffer))) > 0){
                printf("Soy (%d): \n", getpid());
                printf("Me llego: %s\n", buffer);
                write(pipes[1][1],buffer, sizeof(buffer));
            }
            close(pipes[2][0]);
            close(pipes[1][1]);

        while((read(pipes[0][0],buffer,sizeof(buffer))) > 0){
                printf("Soy (%d): \n", getpid());
                printf("Me llego: %s\n", buffer);
                write(pipes[1][1],buffer, sizeof(buffer));
            }
            close(pipes[0][0]);

        //printf("%s\n", entrada);

        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
    }else{
        if (nivel == 1){
            if(child_id == 0){
                for(int i = 0; i < n_tuberia; i++){
                    if(i == 1 || i == 4){
                        close(pipes[i][1]);
                    }else if(i == 0 || i == 5){
                        close(pipes[i][0]);
                    }else{
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }
                }
                while((read(pipes[child_id+1][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[5][1],buffer, sizeof(buffer));
                }
                close(pipes[child_id+1][0]);
                close(pipes[5][1]);
                while((read(pipes[4][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[child_id][1],buffer, sizeof(buffer));
                }
                close(pipes[4][0]);
                close(pipes[child_id][1]);
            }else{
                for(int i = 0; i < n_tuberia; i++){
                    if(i == 3 || i == 6){
                        close(pipes[i][1]);
                    }else if(i == 2 || i == 7){
                        close(pipes[i][0]);
                    }else{
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }
                }
                while((read(pipes[3][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[7][1],buffer, sizeof(buffer));
                }
                close(pipes[3][0]);
                close(pipes[7][1]);
                while((read(pipes[6][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[2][1],buffer, sizeof(buffer));
                }
                close(pipes[6][0]);
                close(pipes[2][1]);
            }
        }else{
            if(child_id == 0){
                for(int i = 0; i < n_tuberia; i++){
                    if(i == 5){
                        close(pipes[i][1]);
                    }else if(i == 4){
                        close(pipes[i][0]);
                    }else{
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }
                }
                while((read(pipes[5][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[4][1],buffer, sizeof(buffer));
                }
                close(pipes[5][0]);
                close(pipes[4][1]);
            }else{
                for(int i = 0; i < n_tuberia; i++){
                    if(i == 7){
                        close(pipes[i][1]);
                    }else if(i == 6){
                        close(pipes[i][0]);
                    }else{
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }
                }
                while((read(pipes[7][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[6][1],buffer, sizeof(buffer));
                }
                close(pipes[7][0]);
                close(pipes[6][1]);
            }
        }
        
    }
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}