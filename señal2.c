#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

void showtree();
void signal_handler(int sig);
int main ( int argc, char **argv){
    pid_t root = getpid();
    int n_children = 2;
    int child_id;
    pid_t children[n_children]; //creamos un vector de pid_t para poder guardar el identificador de cada proceso
    signal(SIGUSR1, signal_handler);

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!children[child_id]){
            break;
        }
    }
    
    if(root == getpid()){ //padre
        showtree();
        printf("Soy padre\n");
        kill(children[n_children-1], SIGUSR1);
        pause();
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
        printf("Ey soy padre wiii\n");
    }else{ // hijo
        pause(); //mimir
        if(child_id == n_children-1){
            printf("Soy hijo 2\n");
            kill(children[child_id-1], SIGUSR1);
        }else{
            printf("Soy hijo 1\n");
            kill(getppid(), SIGUSR1);
        }
    }
    return EXIT_SUCCESS;
}
void signal_handler(int sig){

}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}