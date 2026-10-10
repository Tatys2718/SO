//Carrera de señales
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>

void showtree();
void signal_handler(int sig);
int main(int argc, char **argv){
    pid_t root = getpid();
    int n_children = 5;
    int child_id;
    pid_t children[n_children];

    srand(time(NULL));

    pid_t p_aleatorio = rand() % n_children;

    signal(SIGUSR1, signal_handler);

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!children[child_id]){
            break;
        }
    }

    if(root == getpid()){ //I am dad
        srand(time(NULL));
        pid_t p_aleatorio = rand() % n_children;

        showtree();
        printf("Padre (%d)\n", getpid());
        kill(children[p_aleatorio], SIGUSR1);
        sleep(5);
        for( int j = 0; j < n_children; j++){
            kill(children[j], SIGKILL); //SIGKILL fuerza al proceso a terminar inmediatamente.
            printf("Proceso (%d) murio \n", children[j]);
        }
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
    }else{ //I am children
        pause();
        while(true){
            srand(time(NULL));
            pid_t p_aleatorio = rand() % n_children;
            printf("PID (%d)\n", getpid());
            kill(children[p_aleatorio], SIGUSR1);
            pause();
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