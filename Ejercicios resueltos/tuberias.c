#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>

#define MAX_LEN 64

void showtree();
int main( int argc, char **argv){
    if(argc < 2){
        perror("Envia el numero de hijos y la palabra\n");
        return EXIT_FAILURE;
    }
    pid_t root = getpid();
    int n_children = atoi(argv[1]);
    int child_id;
    int n_pipes = n_children + 1;
    int pipes[n_pipes][2];
    char buffer[MAX_LEN];

    for(int i = 0; i < n_pipes; i++){
        if(pipe(pipes[i]) == -1){
            perror("Fallo la tuberia\n");
            EXIT_FAILURE;
        }
    }

    for(child_id= 0; child_id < n_children; child_id++){
        if(!fork()){
            break;
        }
        
    }
    if(root == getpid()){ //soy padre
        showtree();
        char *msg = argv[2];
        printf("Parent (PID %d) sending the msg: %s\n", getpid(), msg);

        for(int i = 0; i < n_pipes; i++){
            if( i != 0){
                close(pipes[i][1]);
            }
            if(i != n_pipes-1){ 
                close(pipes[i][0]);
            }
        }
        write(pipes[0][1], msg, strlen(msg)+1);
        close(pipes[0][1]);

        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
        read(pipes[n_pipes-1][0], buffer, sizeof(buffer));
        close(pipes[n_pipes-1][0]);
        printf("Parent (PID %d) the msg is: %s\n", getpid(), buffer);
    }else{ //soy hijo 
        for(int i = 0; i < n_pipes; i++){  //tuberia 1 = i
            if(i != child_id){ //hijo 1
                close(pipes[i][0]); //cierra lectura
            }
            if(i != child_id+1){ //hijo 1
                close(pipes[i][1]); //cerramos escritura
            }
        }
        read(pipes[child_id][0], buffer, sizeof(buffer));
        close(pipes[child_id][0]);

        printf("Child (PID  %d) and the msg is: %s\n", getpid(), buffer);

        write(pipes[child_id+1][1], buffer, sizeof(buffer));
        close(pipes[child_id+1][1]);
    }
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}
