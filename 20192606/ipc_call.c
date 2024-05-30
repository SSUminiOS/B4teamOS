#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> // waitpid 함수를 사용하기 위해 추가

#define BUF_SIZE 25
#define READ_END 0
#define WRITE_END 1

int ipc_call() {
    char write_msg[BUF_SIZE] = "Greetings";
    char read_msg[BUF_SIZE];
    int fd[2];
    pid_t pid;

    // 파이프 생성
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    // 자식 프로세스 생성
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    if (pid > 0) {
        // 부모 프로세스
        close(fd[READ_END]); // 부모 프로세스에서는 읽기 끝을 닫음
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1); // 파이프에 쓰기
        close(fd[WRITE_END]); // 쓰기 끝을 닫음
        waitpid(pid, NULL, 0); // 자식 프로세스가 끝날 때까지 대기
    } else {
        // 자식 프로세스
        close(fd[WRITE_END]); // 자식 프로세스에서는 쓰기 끝을 닫음
        read(fd[READ_END], read_msg, BUF_SIZE); // 파이프에서 읽기
        printf("read %s\n", read_msg);
        close(fd[READ_END]); // 읽기 끝을 닫음
        exit(0);
    }

    return 0;
}
