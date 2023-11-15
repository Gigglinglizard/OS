#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <sys/wait.h>

#define MQ_MODE 0666

int main() {
    mqd_t mqd = mq_open("/mymq", O_RDONLY | O_CREAT, MQ_MODE, NULL);
   
    pid_t pid = fork();

    //error in child process
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    //child process, sends the message
    if (pid == 0) {
        char *file_path = "./test.txt"; 

        int file = open(file_path, O_RDONLY);

        int MAX_SIZE = 100000;
        char buf[MAX_SIZE];
        ssize_t bytes_read = read(file, buf, MAX_SIZE);
        
        char *my_mq = "/mymq";

        mqd_t mqd_child = mq_open(my_mq, O_WRONLY);

        mq_send(mqd_child, buf, bytes_read, 0);
        mq_close(mqd_child);

    // parent process, receives the message and counts
    } else {
        int MAX_SIZE = 100000;
        char buf[MAX_SIZE];
        
        ssize_t bytes_received = mq_receive(mqd, buf, MAX_SIZE, NULL);

        int count = 1;
        int i = 0;
        while(buf[i] != 0){
            if(buf[i] == ' '){
                count++;
            }
            i++;
        }

        printf("Amount of words: %d\n", count);
        printf("Message: %s\n", buf);

        mq_close(mqd);
    }

    return 0;
}