#include <stdio.h>   
#include <unistd.h>     
#include <stdlib.h>     
#include <pthread.h>
#include <string.h>
#define MAX_ACCOUNT 2000

typedef struct {
    int tx_id;
    int from_account; 
    int to_account;
    int amount;
} Transfer;

typedef struct {
    int id;
} thread_args;

Transfer *batch;
int n_threads;
int *account_list;
int balance[MAX_ACCOUNT] = {0};
int pending_debit[MAX_ACCOUNT] = {0};
int a_counts;
int l_lotes;
int batch_size;
int all_done;

pthread_mutex_t mutex;
pthread_barrier_t barrier;

void create_threads(const char *file, int n_threads);
void read_file(const char *file);
void *func(void *arg);

int main (int argc, char **argv){
    if(argc < 2){
        perror("Send n_threads");
        return EXIT_FAILURE;
    }
    n_threads = atoi(argv[2]);
    create_threads(argv[1], n_threads);

    return EXIT_SUCCESS;
}
void create_threads(const char *file, int n_threads){
    pthread_t *threads;
    thread_args *args;

    pthread_barrier_init(&barrier, NULL, n_threads+1);
    pthread_mutex_init(&mutex, NULL);

    threads = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
    args = (thread_args*) malloc(n_threads * sizeof(thread_args));
    if(!threads || !args){
        perror("Fail malloc");
        exit(-1);
    }

    for(int i = 0; i < n_threads; i++){
        args[i].id = i;
        pthread_create(&threads[i], NULL, func, &args[i]);
    }

    read_file(file);

    for(int i=0; i < n_threads; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    free(threads);
    free(args);
}
void *func(void *arg){
    thread_args *t = (thread_args*) arg;
    int idx_start;
    int idx_end;
    int chunk;
    int from;
    int to;
    int amount;
    while(1){
        pthread_barrier_wait(&barrier); //inicio
        if(all_done){
            break;
        }
        chunk = batch_size / n_threads;
        idx_start = t->id * chunk;
        if(t->id == n_threads-1){
            idx_end = batch_size;
        }else{
            idx_end = (t->id+1) * chunk; 
        }
        for(int i = idx_start; i < idx_end; i++){
            //printf("Thread: %d working on: %d -> tx_id: %d\n", t->id, i, batch[i].tx_id);
            from = batch[i].from_account;
            to = batch[i].to_account;
            amount = batch[i].amount;

            pthread_mutex_lock(&mutex);
            pending_debit[from] = pending_debit[from] + amount;
            pthread_mutex_unlock(&mutex);
        }
    }


}
void read_file(const char *file){
    FILE *fl = fopen(file, "r");
    if(!fl){
        exit(-1);
    }
    int value;
    int k;
    fscanf(fl, "%d %d", &a_counts, &l_lotes);
    account_list = (int *) malloc(a_counts * sizeof(int));
    if(!account_list){
        exit(-1);
    }
    for(int i=0; i < a_counts; i++){
        fscanf(fl, "%d %d", &account_list[i], &value);
        balance[account_list[i]] = value;
    }
    for(int i = 0; i < l_lotes; i++){
        fscanf(fl, "%d", &k);
        batch_size = k;
        printf("\n----BATCH: %d----\n", i);
        for(int j = 0; j < k; j++){
            fscanf(fl, "%d %d %d %d",
                &batch[j].tx_id,
                &batch[j].from_account,
                &batch[j].to_account,
                &batch[j].amount);
            printf("%d %d %d %d\n",
                batch[j].tx_id,
                batch[j].from_account,
                batch[j].to_account,
                batch[j].amount);
        }
    }
}