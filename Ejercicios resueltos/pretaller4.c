#include <stdio.h>   
#include <unistd.h>     
#include <stdlib.h>     
#include <pthread.h>
#include <string.h>

typedef struct {
    //int id; 
    int user_id;
    int video_id;
    int aspect;
    int value;
} video_sin_reconstruir;

typedef struct{
    int user_id;
    int video_id;
    int face_score;
    int audio_score;
    int sync_score;
} video_construido;

pthread_barrier_t barrier;

void *func(void *arg);
void *func_analista(void *arg);

video_construido *videos;
int *count;
int *usuariosBaneados;
int tam;
int count_sospechosos = 0;
int count_baneados = 0;
int cols = 4;
int main( int argc, char **argv){
    if(argc < 2){
        perror("Se debe enviar el archivo porfavor.\n");
    }

    pthread_barrier_init(&barrier, NULL, 2);
    
    FILE *fl = fopen(argv[1], "r");
    if(!fl){
        perror("Fail fopen\n");
        exit(1);
    }

    fscanf(fl, "%d", &tam);

    video_sin_reconstruir *args;

    args = (video_sin_reconstruir *) malloc(tam * sizeof(video_sin_reconstruir));
    if(!(args)){
        perror("Fail malloc 1\n");
        exit(1);
    }

    videos = (video_construido *) malloc((tam+1) * sizeof(video_construido));
    if(!(videos)){
        perror("Fail malloc 1\n");
        exit(1);
    }

    count = (int *) malloc((tam+1) * sizeof(int));
    if(!(count)){
        perror("Fail malloc 1\n");
        exit(1);
    }

    usuariosBaneados = (int *) malloc((tam+1) * sizeof(int));
    if(!(count)){
        perror("Fail malloc 1\n");
        exit(1);
    }


    printf("Rows: %d - Cols: %d\n", tam, cols);

    for(int i=0; i < tam; i++){
        fscanf(fl, "%d %d %d %d\n", &args[i].user_id, &args[i].video_id, &args[i].aspect, &args[i].value);
        
    }

    for(int i=0; i < tam; i++){
        printf("%d %d %d %d\n", args[i].user_id, args[i].video_id, args[i].aspect, args[i].value);
    }

    pthread_t hilo_reconstructor;


    pthread_create(&hilo_reconstructor, NULL, func, (void*)args);

    pthread_t hilo_analista;

    pthread_create(&hilo_analista, NULL, func_analista, (void*)videos);

    pthread_join(hilo_reconstructor, NULL);
    pthread_join(hilo_analista, NULL);

    pthread_barrier_destroy(&barrier);

    fclose(fl);
    
    free(args);
    args = NULL;
    free(videos);
    videos = NULL;
    free(count);
    count = NULL;
    free(usuariosBaneados);
    usuariosBaneados = NULL;
    return 0;
}

void *func(void *arg){
    video_sin_reconstruir *thread = (video_sin_reconstruir*) arg;
    for(int i = 0; i < tam; i++){
        if(count[thread[i].video_id] < 3){
            videos[thread[i].video_id].user_id = thread[i].user_id;
            videos[thread[i].video_id].video_id = thread[i].video_id;
            if(thread[i].aspect == 1){
                videos[thread[i].video_id].face_score = thread[i].value;
                count[thread[i].video_id] = count[thread[i].video_id] + 1;
            }else if(thread[i].aspect == 2){
                videos[thread[i].video_id].audio_score = thread[i].value;
                count[thread[i].video_id] = count[thread[i].video_id] + 1;
            }else{
                videos[thread[i].video_id].sync_score = thread[i].value;
                count[thread[i].video_id] = count[thread[i].video_id] + 1;
            }
        }
        
    }

    printf("Los videos fueros reconstruidos: \n");

    for(int i = 0; i < tam+1; i++){
        printf("%d %d %d %d %d\n", videos[i].user_id, videos[i].video_id, videos[i].face_score, videos[i].audio_score, videos[i].sync_score);
    }

    printf("El vector del contador: \n");

    for(int i = 0; i < tam+1; i++){
        printf("%d \n", count[i]);
    }

    pthread_barrier_wait(&barrier);

    return NULL;
}

void *func_analista(void *arg){
    video_construido *thread = (video_construido*) arg;
    printf("Entro el analista \n");
    pthread_barrier_wait(&barrier);
    printf("Se rompio la barrera \n");
    for(int i = 0; i < tam+1; i++){
        if((thread[i].face_score >= 80 && thread[i].audio_score >= 75) 
        || (thread[i].face_score >= 80 && thread[i].sync_score <= 35) 
        || (thread[i].audio_score >= 75 && thread[i].sync_score <= 35)){
            count_sospechosos = count_sospechosos + 1;
            usuariosBaneados[thread[i].user_id] = usuariosBaneados[thread[i].user_id] + 1;
        }
    }

    

    for(int i = 0; i < tam+1; i++){
        if(usuariosBaneados[i] > 3){
            printf("USUARIOS BANEADOS: \n");
            count_baneados = count_baneados + 1;
            printf("%d %d \n", i, usuariosBaneados[i]);
        }
    }

    printf("TOTAL BANEADOS: %d\n", count_baneados);
    printf("TOTAL VIDEOS SOSPECHOSOS: %d\n", count_sospechosos);
    

    return NULL;
}