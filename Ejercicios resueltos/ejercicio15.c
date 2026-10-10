//Ejercicio hermandad de tubos: Escriba un programa que dado un número de N de hijos permita
//comunicarlos con un mensaje desde el primer hijo viajando por todos los hermanos hasta llegar
//al ultimo hijo. El esquema debe crear un conjunto de tuberias como lo muestra el diagrama
// (Ejemplo para N=4)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

#define MAX_LEN 64

void showtree();
int main (int argc, char **argv){
    if(argc < 2){
        perror("Debes enviar numero de hijos y el mensaje porfavor\n");
        EXIT_FAILURE;
    }
    pid_t root = getpid();
    int n_children = atoi(argv[1]);
    int child_id;
    int n_tuberias = n_children;
    int in_tuberia [n_tuberias][2];
    int out_tuberia [n_tuberias][2];
    char buffer[MAX_LEN];
    
    for(int i = 0; i < n_tuberias; i++){
        if(pipe(in_tuberia[i]) == -1){
            perror("Fallo la tuberia in\n");
            EXIT_FAILURE;
        }
        if(pipe(out_tuberia[i]) == -1){
            perror("Fallo la tuberia out\n");
            EXIT_FAILURE;
        }
    }
    for(child_id = 0; child_id < n_children; child_id++){
        if(!fork()){
            break;
        }
    }
    if(root == getpid()){ // daddy
        showtree();
        char *msg = argv[2];
        printf("Envia mensaje: %s\n", msg);
        for(int i = 0; i < n_tuberias; i++){
            if( i != 0){
                close(in_tuberia[i][1]);
                close(out_tuberia[i][1]);
                close(in_tuberia[i][0]);
                close(out_tuberia[i][0]);
            }
        }
        write(in_tuberia[0][1], msg, strlen(msg)+1);
        close(in_tuberia[0][1]);
        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }
        read(out_tuberia[0][0], buffer, sizeof(buffer));
        close(out_tuberia[0][0]);
        printf("El mensaje es: %s\n", buffer);
    }else{ //children
        for(int i = 0; i < n_tuberias; i++){
            if(i == child_id){
                close(in_tuberia[i][1]);
                close(out_tuberia[i][0]);
            }else if(i == child_id + 1){
                close(in_tuberia[i][0]);
                close(out_tuberia[i][1]);
            }else{
                close(in_tuberia[i][0]); close(in_tuberia[i][1]);
                close(out_tuberia[i][0]); close(out_tuberia[i][1]);
            }   
            
        }
        
        read(in_tuberia[child_id][0], buffer, sizeof(buffer));
        close(in_tuberia[child_id][0]);
        printf("Child (PID  %d) the msg is: %s\n", getpid(), buffer);
        if(child_id != n_children-1){
            write(in_tuberia[child_id+1][1],buffer, sizeof(buffer));
            close(in_tuberia[child_id+1][1]);
        }else{
            write(out_tuberia[child_id][1],buffer, sizeof(buffer));
            close(out_tuberia[child_id][1]);
        }
        if(child_id != n_children-1){
            read(out_tuberia[child_id+1][0], buffer, sizeof(buffer));
            close(out_tuberia[child_id+1][0]);
            printf("Child (PID  %d) and the msg is: %s\n", getpid(), buffer);
        }
        write(out_tuberia[child_id][1],buffer, sizeof(buffer));
        close(out_tuberia[child_id][1]);
    }
    return EXIT_SUCCESS;
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}