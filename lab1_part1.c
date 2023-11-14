#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    int fd[2];
    pipe(fd); //create pipe

    pid_t fork(void);
    pid_t pid = fork(); //create child

   

    if (pid == -1) {
    perror("fork");
    exit(1);
    }   

    // child process
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1); //kolla vafan andra parametern är
        execlp("ls", "ls", "/", NULL);
    }
    // parent process
    else {
        close(fd[1]);
        dup2(fd[0], 0);
        execlp("wc", "wc", "-l", NULL);
    }
}