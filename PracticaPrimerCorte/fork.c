#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

//void generar(int n);
void showtree();

int main(){
    pid_t root = getpid();
    int n = 2;
    /*for(int i = 0 ; i < n ; i++){
        fork();
        fork();
    }*/

    /*if(fork() && fork()){
        fork();
    }*/
    
    /*pid_t p;
    p = fork();
    if(p != 0){
        fork();
    }
    fork();
    */
    //generar(2);

    /*pid_t p;
    p = fork();
    if(p = 0){
        fork();
    }*/
    
    /*for(int i = 0; i < 3; i++){
        if(!fork()){ //1
            if(i == 0){
                fork(); //2
            }else if(i == 1){
                for(int j = 0; j < 2; j ++){
                    if(!fork()){ //3
                        break;
                    }
                }
            }else{
                for(int k = 0; k < 3 ; k ++){
                    if(!fork()){//4
                        break;
                    }
                }
            }
            break;
        }


    }
    */
    /*for(int i = 0; i < 7; i ++){
        if(!fork()){
            break;
        }
    }*/

    /*for(int i = 0; i < 2; i++){
        if(!fork()){
            if(!fork()){
                if(!fork()){
                    break;
                }
            }break;
        }
    }*/
    /*for (int i = 0; i < 2; i++){
        if(!fork()){
            for(int j = 0; j < 2; j++){
                if(!fork()){
                    break;
                }
            }
            break;
        }
    }
        */
    /*for(int i = 0; i < 3; i++){
        if(!fork()){
            if(i == 1){
                for(int j = 0; j < 2; j++){
                    if(!fork()){
                        break;
                    }
                }
            }else if(i == 2){
                if(!fork()){
                    break;
                }
            }else{
                break;
            }
            break;
        }
    }
        */
    /*
    for(int i = 0; i < 2; i ++){
        if(!fork()){
            if(i == 1){
                for(int j = 0; j < 2;  j ++){
                    if(!fork()){
                        if(j==0){
                            for(int k = 0; k < 2; k ++){
                                if(!fork()){
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
        */
       /*
    if(!fork()){
        for(int i = 0; i < 3; i ++){
            if(!fork()){
                if(i == 1){
                    if(!fork()){
                        for(int j = 0; j < 2; j++){
                            if(!fork()){
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
        */
       /*
    for(int i = 0 ; i < 4 ; i++){
        if(!fork()){
            if(i % 2 == 0){
                for(int j = 0; j < 2 ; j++){
                    if(!fork()){
                        break;
                    }
                }
            }else{
                fork();
            }
            break;
        }
    }
        */
       /*
    for (int i = 0; i < 2; i ++){
        if(!fork()){
            if(i == 0){
                for(int j = 0; j < 2; j++){
                    if(!fork()){
                        if(!fork()){
                            break;
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
        */
    /*
   for(int i = 0; i < 3; i++){
        if(!fork()){
            if(i==0){
                if(!fork()){
                    if(!fork()){
                        break;
                    }
                }
            }else if(i==1){
                for(int j = 0; j < 3; j++){
                    if(!fork()){
                        break;
                    }
                }
            }else{
                break;
            }
            break;
        }
    }
    */
   /*
    for(int i = 0 ; i < 4 ; i++){
        if(!fork()){
            if(i == 0 || i ==3){
                for(int j = 0; j < 2; j++){
                    if(!fork()){
                        break;
                    }
                }
            }
            break;
        }
    }
        */
    /*
    for(int i = 0; i < 3; i++){
        if(i % 2 == 0){
            fork();
        }
    }
        */
    /*
    if(fork() == 0){
        fork();
        if(!fork()) printf ("A\n");
    }else{
        fork();
    }
*/
/*
    for(int i = 0; i < 2; i++){
        fork();
        if(i==0) break;
        fork();
    }
        */
/*
    switch (fork())
    {
    case 0:
        fork();
        break;
    
    default:
        fork();
        break;
    }
*/
/*
    if(fork() || fork()){
        if(fork() == 0){
            fork();
        }
    }
        */
       /*
    pid_t p = fork();
    if(p == 0){
        fork();
    }else{
        if(fork() == 0) fork();
    }
        */
       /*
    for(int i = 0; i < 2; i ++){
        if(fork() ==  0){
            fork();
            break;
        }else{
            fork();
        }
    }
        */
    if(fork() == 0){
        for(int i = 0; i < 2; i++){
            fork();
        }
    }else{
        fork();
    }
    printf("Done\n");

    if(getpid() == root){
        showtree();
        sleep(5);
    }else{
        sleep(1);
    }
    return EXIT_SUCCESS;
}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}
/*
void generar(int n){
    if(n <= 0) return;
    if(fork() == 0){
        generar(n - 1);
    }
    fork();
}
*/
