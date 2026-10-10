//Enviar un mensaje de un archivo de un proceso padre a un proceso hijo
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#define MAX_LEN 64
void read_file (const char *file, char *msg);
int main (int argc, char ** argv){
    if(argc < 2){
        perror("Se necesita un archivo\n");
        return EXIT_FAILURE;
    }
    pid_t root = getpid();
    int tuberia[2];
    char buffer[MAX_LEN];
    if(pipe(tuberia) == -1){
        perror("Error en la tuberia\n");
        return EXIT_FAILURE;
    }
    pid_t hijo = fork();
    if(root == getpid()){
        char msg[MAX_LEN];
        printf("Leer el archivo\n");
        read_file(argv[1], msg);
        printf("El mensaje es: %s\n", msg);
        close(tuberia[0]);
        write(tuberia[1], msg, strlen(msg)+1);
        close(tuberia[1]);
        wait(NULL);
    }else{
        close(tuberia[1]);
        read(tuberia[0], buffer, sizeof(buffer));
        close(tuberia[0]);
        printf("El mensaje del hijo es: %s\n", buffer);
    }

    return EXIT_SUCCESS;
}
void read_file (const char *file, char *msg){
    FILE *f = fopen(file, "r");
    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    char buffer[MAX_LEN];

    fgets(buffer, sizeof(buffer), f);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(msg, buffer);

    fclose(f);
}