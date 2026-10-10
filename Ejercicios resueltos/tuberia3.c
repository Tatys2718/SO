#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LEN 64

int main (int argc, char ** argv){
    pid_t root = getpid();
    int n_pipes = 2;
    int pipes[n_pipes][2];
    char buffer[MAX_LEN];

    for(int i = 0; i < n_pipes; i++){
        if(pipe(pipes[i]) == -1){
            perror("No se pudo crear la tuberia");
            EXIT_FAILURE;
        }
    }

    pid_t hijo = fork();

    if(root == getpid()){ //I am dad
        char *msg = argv[1];
        printf("El mensaje que envio es: %s \n", msg);
        close(pipes[0][0]);
        close(pipes[1][1]);
        write(pipes[0][1],msg, strlen(msg)+1);
        close(pipes[0][1]);
        wait(NULL);
        read(pipes[1][0], buffer, sizeof(buffer));
        close(pipes[1][0]);
        printf("Soy padre y me llego el siguiente mensaje: %s \n", buffer);
    }else{ // I am children
        char msg_vector[MAX_LEN];
        close(pipes[0][1]);
        close(pipes[1][0]);
        read(pipes[0][0], buffer, sizeof(buffer));
        close(pipes[0][0]);
        //printf("El mensaje que recibi como hijo es %s \n", buffer);
        strcpy(msg_vector, buffer);
        printf("El mensaje que me llego es: %s \n", msg_vector);
        for (int i = 0; i < strlen(msg_vector); i++){
            msg_vector[i] = msg_vector[i] - 32;
        }
        printf("El mensaje en mayusculas es: %s \n", msg_vector);
        write(pipes[1][1],msg_vector, strlen(msg_vector)+1);
        close(pipes[1][1]);
    }
    return EXIT_SUCCESS;
}