#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void manejador(int s){
    printf("Signal \n");
}
int Kill(int pid, int s){
    kill(pid, s);
}
int main(int argc, char **argv){
    int n = atoi(argv[1]);
    pid_t child = 0;
    signal();
}
