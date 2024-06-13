#include "process.h"
#include <stdlib.h> // rand() 함수 포함

Process *processes[MAX_PROCESSES];
int process_count = 0;

void create_thread(Process *process, int tid, int burst_time) {
    Thread *new_thread = (Thread *)malloc(sizeof(Thread));
    new_thread->tid = tid;
    new_thread->burst_time = burst_time;
    new_thread->remaining_time = burst_time;
    new_thread->executed_time = 0;
    new_thread->next = NULL;

    if (process->threads == NULL) {
        process->threads = new_thread;
    } else {
        Thread *temp = process->threads;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_thread;
    }
}

void create_process(int id, int arrival_time, int burst_time, int page_count) {
    Process *process = (Process *)malloc(sizeof(Process));
    process->id = id;
    process->arrival_time = arrival_time;
    process->burst_time = burst_time;
    process->remaining_time = burst_time;
    process->threads = NULL;
    process->io_remaining_time = 0;
    process->state = READY; // 초기 상태는 READY
    process->priority = rand() % 3; // 우선순위 설정: 0 (HIGH), 1 (MID), 2 (LOW)

    initialize_page_table(process, page_count);

    // 스레드 생성
    create_thread(process, 1, burst_time); // 예시로 단일 스레드 생성

    // 초기 페이지 로딩
    for (int i = 0; i < page_count && i < 3; i++) { // 첫 3개 페이지 로딩
        request_page(process, i);
    }

    processes[process_count++] = process;
}

void free_processes() {
    for (int i = 0; i < process_count; i++) {
        Thread *current_thread = processes[i]->threads;
        while (current_thread != NULL) {
            Thread *temp = current_thread;
            current_thread = current_thread->next;
            free(temp);
        }
        free(processes[i]->page_table);
        free(processes[i]);
    }
    process_count = 0;
}

void display_processes() {
    printf("ID\tArrival Time\tBurst Time\tRemaining Time\tPriority\n");
    for (int i = 0; i < process_count; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i]->id, processes[i]->arrival_time, processes[i]->burst_time, processes[i]->remaining_time, processes[i]->priority);
        Thread *thread = processes[i]->threads;
        while (thread != NULL) {
            printf("\tThread %d: Burst Time: %d, Remaining Time: %d, Executed Time: %d\n", thread->tid, thread->burst_time, thread->remaining_time, thread->executed_time);
            thread = thread->next;
        }
        printf("  I/O Remaining Time: %d, State: %s\n", processes[i]->io_remaining_time, 
               processes[i]->state == READY ? "READY" : (processes[i]->state == RUNNING ? "RUNNING" : "WAITING"));
    }
}
