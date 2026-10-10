#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

void showtree();
void signal_handler(int sig);
int main (int argc, char **argv){
    pid_t root = getpid();
    int n_children = 3;
    int child_id;
    pid_t children [n_children];
    pid_t nietos[n_children];
    pid_t h;
    pid_t j;
    int nivel = 1;
    signal(SIGUSR1, signal_handler);

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!children[child_id]){
            nietos[child_id] = fork();
            if(!nietos[child_id]){
                nivel = 2;
                if(child_id == 1){
                    h = fork();
                    if(!h){
                        nivel = 3;
                        j = fork();
                        if(!j){
                            nivel = 4;
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
    if(root == getpid()){//Soy padre
        showtree();
        printf("a\n");
        kill(children[n_children-1], SIGUSR1);
        pause();
        printf("a\n");
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
    }else{//Soy hijo
        pause();
        if(child_id == n_children-1){
            if(nivel == 1){
                printf("d\n");
                kill(nietos[child_id], SIGUSR1);
                pause();
                printf("d\n");
                kill(children[1], SIGUSR1);
            }else{
                printf("g\n");
                kill(getppid(), SIGUSR1);
            }
        }else if(child_id == 1){
            if(nivel == 1){
                printf("c\n");
                kill(nietos[child_id], SIGUSR1);
                pause();
                printf("c\n");
                kill(children[0], SIGUSR1);
            }else if(nivel == 2){
                printf("f\n");
                kill(h, SIGUSR1);
                pause(),
                printf("f\n");
                kill(getppid(), SIGUSR1);
            }else if(nivel == 3){
                printf("h\n");
                kill(j, SIGUSR1);
                pause();
                printf("h\n");
                kill(getppid(), SIGUSR1);
            }else{
                printf("j\n");
                kill(getppid(), SIGUSR1);
            }
        }else{
            if(nivel == 1){
                printf("b\n");
                kill(nietos[child_id], SIGUSR1);
                pause();
                printf("b\n");
                kill(getppid(), SIGUSR1);
            }else{
                printf("e\n");
                kill(getppid(), SIGUSR1);
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
void signal_handler(int sig){

}