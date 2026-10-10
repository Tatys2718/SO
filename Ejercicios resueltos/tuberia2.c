#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <string.h>
#define MAX_LEN 64

void showtree();
//void read_file(const char *file, char *msg);
int main (int argc, char **argv){
    pid_t root = getpid();
    
    int n_pipes = 2;
    int pipes [n_pipes][2];
    char buffer [MAX_LEN];

    for(int i = 0; i < n_pipes; i++){
        if(pipe(pipes[i]) == -1){
            perror("Fallo la tuberia\n");
            EXIT_FAILURE;
        }
    }
    pid_t hijo = fork();

    if(root == getpid()){ //I am daddy
        if(argc < 2){
            perror("Envia el nombre del archivo\n");
            EXIT_FAILURE;
        }
        showtree();
        char *msg = argv[1];
        printf("El nombre del archivo es: %s\n", msg);
        close(pipes[0][0]);
        close(pipes[1][1]);
        write(pipes[0][1], msg, strlen(msg)+1);
        close(pipes[0][1]);
        wait(NULL);
        read(pipes[1][0],buffer,sizeof(buffer));
        close(pipes[1][0]);
        printf("Soy padre y me llego este mensaje: %s\n", buffer);

    }else{ //I am children
        char msg[MAX_LEN];
        close(pipes[0][1]);
        close(pipes[1][0]);
        int br = read(pipes[0][0], buffer, sizeof(buffer));
        printf("[%d]Bytes leidos: %d\n", getpid(), br);
        close(pipes[0][0]);
        printf("Soy hijo y me llego: %s\n", buffer);
        FILE *f = fopen(buffer, "r");
        if(f == NULL){
            perror("El archivo no existe\n");
            EXIT_FAILURE;
        }
        char buffer2[MAX_LEN];

        fgets(buffer2, sizeof(buffer2), f);
        buffer2[strcspn(buffer2, "\r\n")] = '\0';
        strcpy(msg, buffer2);
        
        printf("Soy hijo y el mensaje despues de la lectura del archivo es: %s \n", msg);
        write(pipes[1][1], msg, strlen(msg)+1);
        close(pipes[1][1]);
        fclose(f);
    }
    return EXIT_SUCCESS;
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}
/*
void read_file (const char *file, char *msg){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("El archivo no existe\n");
        EXIT_FAILURE;
    }
    char buffer[MAX_LEN];

    fgets(buffer, sizeof(buffer), f);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(msg, buffer);

    fclose(f);
}*/