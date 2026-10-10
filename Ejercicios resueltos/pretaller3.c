#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>

#define MAX 100

void showtree();
int is_suspisous(Data data);

typedef struct(){
    int video_id;
    int user_id;
    int face_score, audio_score, sync_score;
    int origin;

    int partial_risk;
    int is_suspisous; //0 = no, 1 = si es sospechoso
}Data;

typedef struct {
    int user_id;
    int count;
} Result;

int main(int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo\n");
        EXIT_FAILURE;
    }
    pid_t root = getpid();
    int n_children = 3;
    int child_id;
    int n_pipes = n_children;
    int in_pipes[n_children][2];
    int out_pipes[n_children][2];

    for(int i=0; i < n_pipes; i++){
        pipe(in_pipes[i]);
        pipe(out_pipes[i]);
    }

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){ //SOY PADRE
        showtree();
        for(int i=0; i < n_pipes; i++){
            close(in_pipes[i][0]);
            close(out_pipes[i][1]);
        }
        int N;
        int B;
        int turn = 0;
        int total_susp;
        int total_bann;

        Data data;
        Data recived;
        Result results = {0};
        Result total[MAX];

        FILE *fl;
        total_susp = total_bann = 0;
        fl = fopen(argv[1], "r");

        if(!fl) {
            perror("Error al abrir el archivo");
            exit(1);
        }

        fscanf(fl, "%d %d", &B, &N);
        printf("B: %d, N: %d\n", B, N);

        for(int i=0; i < n_pipes; i++){ //Le paso a los tres hijos el valor de B
            write(in_pipes[i][1], &B, sizeof(int));
        }

        for(int i=0; i < N/B; i++){//Recorre por bloque digamos N = 12 y B = 3 -> 12/3=4, recorrera 4 bloques
            for(int j=0; j < B; j++){ //Recorrera la fila de cada bloque
                fscanf(fl, "%d %d %d %d %d %d", &data.video_id,
                    &data.user_id, &data.face_score, &data.audio_score,
                    &data.sync_score, &data.origin
                );//del archivo va a a escanear los datos por linea
                write(in_pipes[turn][1], &data, sizeof(Data)); //Escribira en cada hijo según el turno, si es turno 0 entonces al hijo 0...
                turn++;//aumenta el turno para el siguiente hijo

                if(turn == n_children){ //Si el turno llega al numero total de hijos es porque ya los 3 hijos recibieron datos
                    turn = 0; //El turno se reinicia
                }
            }
            //printf("\nRound: %d\n", i);
        }
        for(int i=0; i < n_pipes; i++){
            close(in_pipes[i][1]);
        }
        memset(total, 0, sizeof(total));

        for(int i=0; i < n_pipes; i++){
            while(read(out_pipes[i][0], &results, sizeof(results)) > 0){//El padre esta leyendo los resultados
                if(results.user_id == -1) break;

                total[results.user_id].user_id = results.user_id;
                total[results.user_id].count += results.count;
            }
        }
        for(int i=0; i < n_pipes; i++){
            close(out_pipes[i][0]);
        }

        fclose(fl);

        for(int i=0; i < MAX; i++){
            if(total[i].count > 0){
                total_susp += total[i].count;

                if(total[i].count > 3){
                    printf("User: %d, Count: %d\n", i,  total[i].count);
                    total_bann++;
                }
            }
        }

        for(int i = 0; i < n_children; i++){
            wait(NULL);
        }

        printf("All banned: %d\n", total_bann);
        printf("All suspisous: %d\n", total_susp);
        
    }else{//SOY HIJO
        for(int i=0; i < n_pipes; i++){
            if(i == child_id){
                close(in_pipes[i][1]);
                close(out_pipes[i][0]);
            }else{
                close(in_pipes[i][0]); close(in_pipes[i][1]);
                close(out_pipes[i][0]); close(out_pipes[i][1]);
            }
        }
        int B, lines = 0;
        Data recived;
        Result resp[MAX];
        memset(resp, 0, sizeof(resp));

        read(in_pipes[child_id][0], &B, sizeof(int)); //Cada hijo recibe la variable B

        while((read(in_pipes[child_id][0], &recived, sizeof(Data))) > 0){ //Mientra que a cada hijo le este llegando algo para leer, tambien le esta llegando una fila completa, un video completo
            if(is_suspisous(recived)){ //Verifica si el video que recibio es sospechoso
                resp[recived.user_id].user_id = recived.user_id; //Si lo es, agrega el user id al vector de resp
                resp[recived.user_id].count++;//y aumenta el contador del vector.
            }
            lines++; //Continua a la siguiente linea
            if(lines == B){ //Si llegamos a la linea final del bloque
                for(int i = 0; i < MAX; i++){ //Recorremos el vector donde se almacenan los usuarios sospechosos
                    if(resp[i].count > 0){ //si en cada usuario sospechoso su contados es mayor que 0
                        write(out_pipes[child_id][1], &resp[i], sizeof(Result)); //Escribe en la tuberia de salida de cada hijo 
                    }//duda: aqui un hijo no podria estar escribiendo el resultado de otro hijo?

                }
                lines = 0;
                memset(resp, 0, sizeof(resp));
            }
            close(in_pipes[child_id][0]); //cerramos tuberias de lectura
            for(int i=0; i < MAX; i++){ //recorremos el max
                if(resp[i].count > 0){ //al parecer va a escribir los resultados que sobraron
                    write(out_pipes[child_id][1], &resp[i], sizeof(Result));
                }
            }
            Result end = {-1, -1};
            write(out_pipes[child_id][1], &end, sizeof(Result)); //Los hijos escriben que ya terminaron

            close(out_pipes[child_id][1]);


        }
    

        return EXIT_SUCCESS;

    }




}
void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}

int is_suspisous(Data data){
    int is = 0;
    if(data.face_score >= 80) is++;
    if(data.audio_score >= 75) is++;
    if(data.sync_score <= 35) is++;
    return is >= 2;
}