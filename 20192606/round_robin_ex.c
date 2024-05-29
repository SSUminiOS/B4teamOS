#include <stdio.h>

#define MAX_PROCESS 10

struct Process {
    int pid;
    int arrival_time;//도착시간
    int burst_time;//실행시간
    int remaining_time;//남은시간
};

void round_robin(struct Process processes[], int n, int quantum) {
    int total_time = 0;//모든 프로세스가 실행될 시간을 더한 변수
    for (int i = 0; i < n; i++) {
        total_time += processes[i].burst_time;
    }

    int time = 0;
    while (total_time > 0) {
        for (int i = 0; i < n; i++) {
            struct Process *p = &processes[i];

            //실행 완료
            if (p->remaining_time <= 0) {
                continue;
            }

            //마지막 남은 시간 처리
            if (p->remaining_time <= quantum) 
            {
                time += p->remaining_time;
                total_time -= p->remaining_time;
                p->remaining_time = 0;
            } 
            

            //quantum만큼 처리
            else
            {
                time += quantum;
                total_time -= quantum;
                p->remaining_time -= quantum;
            }
            printf("Process %d is running at time %d\n", p->pid, time);
        }
    }
}

int round_robin_ex() {
    struct Process processes[MAX_PROCESS] = {
        {1, 0, 10, 10},
        {2, 1, 4, 4},
        {3, 2, 3, 3},
        {4, 3, 7, 7}
    };
    int n = 4; // 프로세스의 수
    int quantum = 2; // 타임 슬라이스 크기
    round_robin(processes, n, quantum);
    return 0;
}
