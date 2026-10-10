#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>

void showtree();
void signal_handler(int sig);
int main (int argc, char **argv){
    pid_t root = getpid();
    int n_children = 3;
    int child_id;
    pid_t children[n_children];
    int nivel = 1;
    pid_t f;
    pid_t g;
    pid_t i;
    pid_t j;
    signal(SIGUSR1, signal_handler);

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!children[child_id]){
            if(child_id == 0){
                f = fork();
                if(!f){
                    nivel = 2;
                    g = fork();
                    if(!g){
                        nivel = 3;
                        break;
                    }
                }
            }else if(child_id == 2){
                i = fork();
                if(!i){
                    nivel = 2;
                    j = fork();
                    if(!j){
                        nivel = 3;
                        break;
                    }
                }
            }break;
        }
    }

    if( root == getpid()){ //I'm dad wiii
        showtree();
        printf("a\n");
        kill(children[n_children-1], SIGUSR1);
        pause();
        for(int i = 0; i<n_children; i ++){
            wait(NULL);
        }
        printf("a\n");
    }else{ //I'm children wiiu
        pause();
        if(child_id == n_children-1){
            if(nivel == 1){
                printf("e\n");
                kill(i, SIGUSR1);
                pause();
                printf("e\n");
                kill(children[1], SIGUSR1);
            }else if(nivel == 2){
                printf("i\n");
                kill(j, SIGUSR1);
                pause();
                printf("i\n");
                kill(getppid(), SIGUSR1);
            }else{
                printf("j\n");
                kill(getppid(), SIGUSR1);
            }
        }else if(child_id == 1){
            printf("d\n");
            kill(children[0], SIGUSR1);
        }else{
            if(nivel == 1){
                printf("c\n");
                kill(f, SIGUSR1);
                pause();
                printf("c\n");
                kill(getppid(),SIGUSR1);
            }else if(nivel == 2){
                printf("f\n");
                kill(g, SIGUSR1);
                pause();
                printf("f\n");
                kill(getppid(), SIGUSR1);
            }else{
                printf("g\n");
                kill(getppid(), SIGUSR1);
            }
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