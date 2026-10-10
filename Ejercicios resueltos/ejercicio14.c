//Enviar un vector de enteros de un proceso padre a un proceso hijo a traves de señales
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
void showtree();
void signal_handler(int sig);
int flag = 1;//SI BANDERA ES IGUAL A 1 NO E TERMINADO DE ENVIAR SEÑAL, PERO SI BANDERA ES IGUAL A 0 TERMINE DE ENVIAR UN NUMERO DEL VECTOR
int main (int argc, char**argv){
    pid_t root = getpid();
    pid_t hijo = fork();
    int count = 0;
    int count_size = 0;
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    if(root == getpid()){ //I am dad
        showtree();
        int vector[] = {1,2,3,4,5};
        int n = 5;
        for(int i = 0; i<n; i++){
            printf("%d ", vector[i]);
        }
        printf("\n");

        //Enviemos señal del size del vector
        printf("Padre: envio señal del size al hijo \n");
        for(int i = 0; i < n; i++){
            printf("Señal 1\n");
            kill(hijo, SIGUSR1);
            pause();
        }

        kill(hijo, SIGUSR2);
        pause();

        for(int i = 0; i<n; i++){
            for(int j = 0; j < vector[i]; j++){
                //printf("Envio señal 2\n");
                kill(hijo, SIGUSR1);
                pause();
                //printf("Me llego señal 3\n");
            }
            //printf("Envio señal 4\n");
            kill(hijo, SIGUSR2);
            pause();
        }

        kill(hijo, SIGUSR2);
        pause();
    
        wait(NULL);
        printf("War is over\n");
    }else{ //I am child wiiu
        pause();
        while(flag == 1){
            printf("Recibida señal 1\n");
            count_size = count_size + 1;
            kill(getppid(), SIGUSR1);
            pause();
        }
        printf("Hijo: Me llego que el size del vector es de: %d \n", count_size);
        kill(getppid(), SIGUSR1);
        pause();
        //printf("Me llego señal 2\n");
        int vector_hijo[count_size];
        int f = 0;
        while(flag == 1 && f < count_size){
            count = count + 1;
            //printf("Hujo envio señal 3 \n");
            kill(getppid(), SIGUSR1);
            pause();
            if(flag == 0){
                //printf("Me llego señal 4\n");
                vector_hijo[f] = count;
                count = 0;
                f++;
                kill(getppid(), SIGUSR1);
                pause();
                }
            }
        
        for(int k = 0; k < count_size; k++){
            printf("%d ", vector_hijo[k]);
        }
        printf("\n");
        kill(getppid(), SIGUSR2);
    }
    
    return EXIT_SUCCESS;
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}
void signal_handler(int sig){
    if(sig == SIGUSR1) {
        flag = 1;
    } 
    else if(sig == SIGUSR2) {
        flag = 0;
    }
}

