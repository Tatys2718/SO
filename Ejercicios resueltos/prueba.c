#include<stdlib.h>
#include<stdio.h>

int main(){
    int numero = 1234;
    FILE *f;
    f = fopen("archivo.txt", "a");
    if(f == NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }
    fprintf(f, "%d", numero);

    fclose(f);
    printf("Entero anexado exitosamente.\n");
    return 0;
}