
#include <bits/pthreadtypes.h>
#include <cstddef>

int counter = 0;
pthread_mutex_t lock;

void* thread_func(void *arg) {
    pthread_mutex_lock(&lock);
    counter++;
    printf("Job %d started.\n", counter);
    sleep(2);
    printf("Job %d finished.\n", counter);
    pthread_mutex_unlock(&lock);

    return NULL;
}


    int main(void) {
        pthread_t t1, t2;
        pthread_mutex_init(&lock, NULL);
        pthread_create(&t1, NULL, &thread_func, NULL);
        pthread_create(&t2, NULL, &thread_func, NULL);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_mutex_destroy(&lock);
        return 0;
}