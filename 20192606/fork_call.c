#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void fork_call() {
    pid_t pid;

    //call fork() make child process
    pid = fork();

    if (pid < 0) { //fork failed
        fprintf(stderr, "fork failed\n");
    }
    else if (pid == 0) { //child process
        printf("Hello it's from Child process (PID: %d)\n", getpid());
        execlp("/bin/ls","ls",NULL); //child != parent now
    }
    else { //parent process
        wait(NULL); //transit to wait state
        printf("Chiild complete.\n");
        printf("Hello it's from Parent process (PID: %d)\n", getpid());
        printf("Child PID: %d\n", pid);
    }
}
