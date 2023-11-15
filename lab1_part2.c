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
    // create message queue
    mqd_t mqd = mq_open("/mymq", O_RDONLY | O_CREAT, MQ_MODE, NULL);

    // create child
    pid_t pid = fork();

    // catch errors in fork
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    // child process
    if (pid == 0) {
        char *my_mq = "/mymq";
        char *write_msg = "hello";
        mqd_t mqd_child;
        // Open an existing message queue
        mqd_child = mq_open(my_mq, O_WRONLY);
        // Write "hello" to the message queue
        mq_send(mqd_child, write_msg, strlen(write_msg), 0);
        // Close the message queue
        mq_close(mqd_child);
    }

    // parent process
    else {
        wait(NULL);
        int MAX_SIZE = 100;
        int MAX_NUM_MSG = 10;
        char *my_mq = "/mymq";
        char buf[MAX_SIZE];
        mqd_t mqd_parent;
        struct mq_attr attr;
        // Form the queue attributes
        attr.mq_maxmsg = MAX_NUM_MSG;
        attr.mq_msgsize = MAX_SIZE;
        // Create message queue
        mqd_parent = mq_open(my_mq, O_RDONLY | O_CREAT, MQ_MODE, &attr);
        // Read the message from the message queue
        mq_receive(mqd_parent, buf, MAX_SIZE, NULL);
        printf("Message: %s\n", buf);
        // Close the message queue
        mq_close(mqd_parent);
        // Remove the message queue
        mq_unlink("/mymq");
    }

    return 0;
}
