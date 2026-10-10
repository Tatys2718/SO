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
    int n_pipes = 4;
    int pipes[n_pipes][2];
    char buffer[MAX_LEN];
    int nivel = 1;
    int child_id;
    int fin = 0;


    for(int i = 0; i < n_pipes; i++){
        if(pipe(pipes[i]) == -1){
            perror("Error en la creación de la tuberia");
            EXIT_FAILURE;
        }
    }

    for (child_id = 0; child_id < n_children ; child_id++){
        if(!fork()){
            if(!fork()){
                nivel = 2;
                break;
            }
            break;
        }
    }

    if (root == getpid()){ //Soy padre
        char entrada [MAX_LEN];
        showtree();
        for(int i = 0; i < n_pipes; i++){
            close(pipes[i][0]);
            if( i != 0){
                close(pipes[i][1]);
            }
        }
        while(!fin){
            scanf ("%s", entrada);
            if(strcmp(entrada, "fin") == 0){
                fin = 1;
                break;
            }
            write(pipes[0][1], entrada, strlen(entrada)+1);
        }
        close(pipes[0][1]);

        printf("%s\n", entrada);
        
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
    }else{ //Soy hijo
        if (child_id == 0 || child_id == 1){
            if(nivel == 1){
                for(int i = 0; i <n_pipes; i++){
                    if(i != child_id){
                        close(pipes[i][0]);
                    }
                    if(i != child_id + 1){
                        close(pipes[i][1]);
                    }

                }
            }else{
                if(child_id == 0){
                    for(int i = 0; i <n_pipes; i++){
                        if(i != nivel){
                            close(pipes[i][0]);
                        }
                        if(i != nivel + 1){
                            close(pipes[i][1]);
                        }
                    }
                }else{
                    for(int i = 0; i <n_pipes; i++){
                        if(i != nivel+1){
                            close(pipes[i][0]);
                        }
                        close(pipes[i][1]);
                    }
                }
            }
            
        }

        if(nivel == 1){
            while((read(pipes[child_id][0],buffer,sizeof(buffer))) > 0){ //Mientras read me diga que leyo algo siga ejecutando
                printf("Soy (%d): \n", getpid());
                printf("Me llego: %s\n", buffer);
                write(pipes[child_id+1][1],buffer,sizeof(buffer));
            }
            close(pipes[child_id][0]);
            close(pipes[child_id+1][1]);
            
        }else{
            if(child_id == 0){
                while((read(pipes[nivel][0],buffer,sizeof(buffer))) > 0){
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);
                    write(pipes[nivel+1][1],buffer, sizeof(buffer));
                }
                close(pipes[nivel][0]);
                close(pipes[nivel+1][1]);
            }else{
                while((read(pipes[nivel+1][0],buffer,sizeof(buffer))) > 0){
                    if(strcmp(buffer, "fin") == 0){
                        break;
                    }
                    printf("Soy (%d): \n", getpid());
                    printf("Me llego: %s\n", buffer);  
                }
                close(pipes[nivel+1][0]);
            }
        }
    
    }
    
    return EXIT_SUCCESS;
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}