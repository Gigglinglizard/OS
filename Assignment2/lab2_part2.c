#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 2

// Shared data structure
typedef struct {
    int var;
    sem_t mutex;
    sem_t writeMutex;
    int readerCount;
} SharedData;

SharedData *sharedData;

void initializeSharedData() {
    sharedData = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sharedData == MAP_FAILED) {
        perror("Error in mmap");
        exit(EXIT_FAILURE);
    }

    sharedData->var = 0;
    sem_init(&sharedData->mutex, 1, 1);  // Initialize mutex to 1 (unlocked)
    sem_init(&sharedData->writeMutex, 1, 1);  // Initialize writeMutex to 1 (unlocked)
    sharedData->readerCount = 0;
}

void cleanupSharedData() {
    sem_destroy(&sharedData->mutex);
    sem_destroy(&sharedData->writeMutex);
    munmap(sharedData, sizeof(SharedData));
}

void* writer(void* arg) {
    while (1) {
        sem_wait(&sharedData->writeMutex);

        // Write to the shared buffer
        sharedData->var++;
        printf("The writer (PID %d) writes the value %d\n", getpid(), sharedData->var);

        // Release the lock
        sem_post(&sharedData->writeMutex);

        if (sharedData->var == MAX) {
            break;
        }

        // Introduce delay to make messages visible
        sleep(1);
    }

    return NULL;
}

void* reader(void* arg) {
    sem_wait(&sharedData->mutex);
    sharedData->readerCount++;
    if (sharedData->readerCount == 1) {
        sem_wait(&sharedData->writeMutex);
        printf("The first reader acquires the lock.\n");
    }
    sem_post(&sharedData->mutex);

    // Read from the shared buffer
    printf("Reader (PID %d) reads the value %d\n", getpid(), sharedData->var);

    sem_wait(&sharedData->mutex);
    sharedData->readerCount--;
    if (sharedData->readerCount == 0) {
        sem_post(&sharedData->writeMutex);
        printf("The last reader releases the lock.\n");
    }
    sem_post(&sharedData->mutex);

    return NULL;
}

int main() {
    initializeSharedData();

    pid_t writerPID, reader1PID, reader2PID;

    // Create the writer process
    if ((writerPID = fork()) == 0) {
        writer(NULL);
        exit(EXIT_SUCCESS);
    }

    // Create the first reader process
    if ((reader1PID = fork()) == 0) {
        reader(NULL);
        exit(EXIT_SUCCESS);
    }

    // Create the second reader process
    if ((reader2PID = fork()) == 0) {
        reader(NULL);
        exit(EXIT_SUCCESS);
    }

    // Wait for all child processes to finish
    waitpid(writerPID, NULL, 0);
    waitpid(reader1PID, NULL, 0);
    waitpid(reader2PID, NULL, 0);

    cleanupSharedData();

    return 0;
}
