#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX 5

struct SharedMemory {
    int data;
    sem_t mutex;
    sem_t writeMutex;
    int readCount;
};

int main() {
    // Initialize shared memory
    struct SharedMemory *shared = mmap(NULL, sizeof(struct SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared->data = 0;
    shared->reader_count = 0;
    sem_init(&shared->mutex, 1, 1);
    sem_init(&shared->writeMutex, 1, 1);


//write
    do {
        sem_wait(&shared->writeMutex);
    
        /* writing is performed */
    
        sem_post(&shared->writeMutex);
    } while (1);

//read
do {
    sem_wait(&shared->mutex);
    shared->readCount++;
    if (readCount == 1){
    sem_wait(&shared->writeMutex);
    }
    sem_post(&shared->mutex);
    /* reading is performed */


    sem_wait(&shared->mutex);
    shared->readCount--;
    if (&shared->readCount == 0){
    sem_post(&shared->writeMutex);
    }
    sem_post(&shared->mutex);
} while (1);












    // Create processes for writer and readers
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {  // Child process
            if (i == 0) {  // Writer process
                while (shared->data < MAX) {
                    sem_wait(&shared->writeMutex);
                    printf("Writer (%d) writes data: %d\n", getpid(), ++shared->data);
                    sem_post(&shared->writeMutex);
                    sleep(1);
                }
            } else {  // Reader processes
                while (shared->data < MAX) {
                    sem_wait(&shared->mutex);
                    shared->reader_count++;
                    if (shared->reader_count == 1) {
                        sem_wait(&shared->writeMutex);
                        printf("First reader (%d) acquired the lock\n", getpid());
                    }
                    sem_post(&shared->mutex);

                    printf("Reader (%d) reads data: %d\n", getpid(), shared->data);

                    sem_wait(&shared->mutex);
                    shared->reader_count--;
                    if (shared->reader_count == 0) {
                        printf("Last reader (%d) released the lock\n", getpid());
                        sem_post(&shared->wrt);
                    }
                    sem_post(&shared->mutex);
                    sleep(1);
                }
            }
            exit(0);  // Child process exits
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    // Clean up
    sem_destroy(&shared->mutex);
    sem_destroy(&shared->wrt);
    munmap(shared, sizeof(struct SharedMemory));

    return 0;
}
