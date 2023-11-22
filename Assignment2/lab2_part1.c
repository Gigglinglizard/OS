
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3
#define MAX_ITERATIONS 15

int buffer = 0;
int thread_counters[NUM_THREADS] = {0};

pthread_mutex_t lock;

void* thread_func(void *arg) {
    int thread_index = *(int*)arg;
    int done = 0;

    //while(!done){
        pthread_mutex_lock(&lock);

        //done = buffer >= 15;
        //if(!done){
            if(buffer%NUM_THREADS == thread_index && buffer < MAX_ITERATIONS){
                printf("TID: %ld, PID: %d, Buffer: %d\n", pthread_self(), getpid(), buffer);
                
                thread_counters[thread_index]++;
                buffer++;
                sleep(1);
            }
        //}
        pthread_mutex_unlock(&lock); 
    //}
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int thread_indices[NUM_THREADS] = {0, 1, 2};

    pthread_mutex_init(&lock, NULL);
    
    for(int i = 0; i < MAX_ITERATIONS; i++){
        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_create(&threads[i], NULL, &thread_func, &thread_indices[i]);
        }

        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_join(threads[i], NULL);
        }

        if(buffer >= MAX_ITERATIONS){
            break;
        }
}

    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("TID %ld worked on the buffer %d times\n", threads[i], thread_counters[i]);
    }

    printf("Total buffer accesses: %d\n", buffer);

    pthread_mutex_destroy(&lock);
    return 0;
}