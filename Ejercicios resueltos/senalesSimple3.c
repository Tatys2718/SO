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
    int n_children = 5;
    int child_id;
    pid_t children[n_children];
    pid_t nietos[n_children-2];
    int nivel = 1;
    signal(SIGUSR1, signal_handler);

    for(child_id = 0; child_id < n_children; child_id++){
        children[child_id] = fork();
        if(!children[child_id]){
            if(child_id % 2 == 0){
                nietos[child_id] = fork();
                if(!nietos[child_id]){
                    nivel = 2;
                    break;
                }
            }
            break;
        }
    }
    if(root == getpid()){
        showtree();
        printf("a\n");
        kill(children[n_children-1], SIGUSR1);
        pause();
        for(int i = 0; i < n_children ; i++){
            wait(NULL);
        }
        printf("a\n");

    }else{
        pause();
        if(child_id % 2 == 0){
            if(nivel == 1){
                printf("pid(%d)\n", getpid());
                kill(nietos[child_id], SIGUSR1);
                pause();
                if(child_id == 0){
                    printf("pid(%d)\n", getpid());
                    kill(getppid(), SIGUSR1);
                }else{
                    printf("pid(%d)\n", getpid());
                    kill(children[child_id-1], SIGUSR1);
                } 
            }else{
                printf("pid(%d)\n", getpid());
                kill(getppid(), SIGUSR1);
            }
        }else{
            printf("pid(%d)\n", getpid());
            kill(children[child_id-1], SIGUSR1);
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