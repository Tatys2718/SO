#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

void showtree();
void signal_handler(int sig);
int main (int argc, char **argv){
    pid_t root = getpid();
    int n_children = 3;
    int child_id;
    pid_t children[n_children];
    pid_t h;
    pid_t g;
    int nivel = 1;
    signal(SIGUSR1, signal_handler);

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!children[child_id]){
            if(child_id == 2){
                h = fork();
                if(!h){
                    nivel = 2;
                    break;
                }
            }else if(child_id == 1){
                g = fork();
                if(!g){
                    nivel = 2;
                    break;
                }
            }break;
        }
    }

    if(root == getpid()){ //I am dad
        showtree();
        printf("a\n");
        kill(children[n_children-1], SIGUSR1);
        pause();
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
        printf("a\n");

    }else{ //I am children
        pause();
        if(child_id == n_children-1){
            if(nivel == 1){
                printf("f\n");
                kill(h, SIGUSR1);
                pause();
                printf("f\n");
                kill(children[1], SIGUSR1);
            }else{
                printf("h\n");
                kill(getppid(), SIGUSR1);
            }
        }else if(child_id == 1){
            if(nivel == 1){
                printf("e\n");
                kill(g, SIGUSR1);
                pause();
                printf("e\n");
                kill(children[0], SIGUSR1);
            }else{
                printf("g\n");
                kill(getppid(), SIGUSR1);
            }
        }else{
            printf("d\n");
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