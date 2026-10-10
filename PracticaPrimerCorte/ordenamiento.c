#include <stdlib.h>
#include <stdio.h>

void ordenar(int vector[], int n);
int main (int argc, char **argv){
    int v[5] = {3,2,6,1,7};
    int n = 5;
    ordenar(v,n);

    for(int i = 0; i < n; i++){
        printf("%d", v[i]);
    }
    printf ("\n");
}
void ordenar(int vector[], int n){
    int aux;
    for(int i = 0 ; i < n; i++){
        for(int j = i+1; j < n; j++){
            if (vector[j] < vector[i]){
                aux = vector[j];
                vector[j] = vector[i];
                vector[i] = aux;
            }
        }
    }
}