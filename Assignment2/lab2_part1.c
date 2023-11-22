
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

int buffer = 0;
int thread_counters[NUM_THREADS] = {0};

pthread_mutex_t lock;

void* thread_func(void *arg) {
    int thread_index = *(int*)arg;
    int done = 0;

    while(!done){
        pthread_mutex_lock(&lock);

        done = buffer >= 15;
        if(!done){
            printf("TID: %ld, PID: %d, Buffer: %d\n", pthread_self(), getpid(), buffer);
            
            thread_counters[thread_index]++;
            buffer++;
        }
        pthread_mutex_unlock(&lock); 
    }
    sleep(1);
    
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int thread_indices[NUM_THREADS] = {0, 1, 2};

    pthread_mutex_init(&lock, NULL);

    pthread_create(&threads[0], NULL, &thread_func, &thread_indices[0]);
    pthread_create(&threads[1], NULL, &thread_func, &thread_indices[1]);
    pthread_create(&threads[2], NULL, &thread_func, &thread_indices[2]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("TID %ld worked on the buffer %d times\n", threads[i], thread_counters[i]);
    }

    printf("Total buffer accesses: %d\n", buffer);

    pthread_mutex_destroy(&lock);
    return 0;
}