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
    char buffer[100][MAX_LEN];
    int nivel = 1;
    int child_id;

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
        showtree();
    }else{
        sleep(1);
    }
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}