#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX 3
#define PROCESS_NUM 3

struct SharedMemory {
    int data;
    sem_t mutex;
    sem_t writeMutex;
    int readCount;
};

struct SharedMemory *sharedData;

void initializeSharedData() {
    sharedData = mmap(NULL, sizeof(struct SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sharedData->data = 0;
    sem_init(&sharedData->mutex, 1, 1);         // Initialize mutex to 1 (unlocked)
    sem_init(&sharedData->writeMutex, 1, 1);   // Initialize writeMutex to 1 (unlocked)
    sharedData->readCount = 0;
}

int writer() {
    sem_wait(&sharedData->writeMutex);
    printf("Writer (%d) writes data: %d\n", getpid(), ++sharedData->data);
    sem_post(&sharedData->writeMutex);
    sleep(1);
    return 0;
}

int reader() {
    sem_wait(&sharedData->mutex);
    sharedData->readCount++;
    if (sharedData->readCount == 1) {
        sem_wait(&sharedData->writeMutex);
        printf("First reader (%d) acquired the lock\n", getpid());
    }
    sem_post(&sharedData->mutex);

    printf("Reader (%d) reads data: %d\n", getpid(), sharedData->data);

    sem_wait(&sharedData->mutex);
    sharedData->readCount--;
    if (sharedData->readCount == 0) {
        printf("Last reader (%d) released the lock\n", getpid());
        sem_post(&sharedData->writeMutex);
    }
    sem_post(&sharedData->mutex);
    sleep(1);
    return 0;
}

int main() {
    initializeSharedData();

    int i = 0;
    while (i < PROCESS_NUM) {
        pid_t pid = fork();
        if (pid == 0) {  // Child process
            if (i == 0) {  // Writer process
                while (sharedData->data < MAX) {
                    writer();
                }
            } else {
                while (sharedData->data < MAX) {
                    reader();
                }
            }
            exit(0);  
        }
        i++;
    }

    for (int i = 0; i < PROCESS_NUM; i++) {
        wait(NULL);
    }

    sem_destroy(&sharedData->mutex);
    sem_destroy(&sharedData->writeMutex);
    munmap(sharedData, sizeof(struct SharedMemory));

    return 0;
}