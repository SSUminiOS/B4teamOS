#include "schedule.h"
#include "stats.h"
#include "process.h"
#include "memory.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#define HIGH_PRIORITY_WEIGHT 3
#define MID_PRIORITY_WEIGHT 2
#define LOW_PRIORITY_WEIGHT 1

extern Process *processes[MAX_PROCESSES];
extern int process_count;
extern FrameTableEntry *frame_table;
extern int frame_count;

Process *high_priority_queue = NULL;
Process *mid_priority_queue = NULL;
Process *low_priority_queue = NULL;

// Define colors
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_RED "\033[91m" // 인터럽트 발생한 프로세스를 표시할 색상

// Function to get terminal size
void get_terminal_size(int *rows, int *cols) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *rows = w.ws_row;
    *cols = w.ws_col;
}

// Clear the terminal screen
void clear_screen() {
    printf("\033[2J");
    printf("\033[H");
}

void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

void display_progress() {
    int rows, cols;
    get_terminal_size(&rows, &cols);
    clear_screen();
    for (int i = 0; i < process_count; i++) {
        Process *process = processes[i];
        move_cursor(i * 5 + 1, 1);
        if (process->was_interrupted) {
            printf("%sProcess %d%s\n", BRIGHT_RED, process->id, RESET);
        } else {
            printf("%sProcess %d%s\n", BRIGHT_BLUE, process->id, RESET);
        }
        Thread *thread = process->threads;
        while (thread != NULL) {
            printf("  Thread %d: %s", thread->tid, GREEN);
            for (int j = 0; j < thread->burst_time; j++) {
                if (j < thread->executed_time) {
                    printf("#");
                } else {
                    printf("-");
                }
            }
            if (thread->remaining_time == 0) {
                printf("."); // 프로세스 완료 표시
            }
            printf("%s %d/%d\n", RESET, thread->executed_time, thread->burst_time);
            thread = thread->next;
        }
    }
/*
    // 물리 메모리 프레임을 표시
    int start_col = cols - 20; // 오른쪽으로 25 컬럼만큼 이동
    move_cursor(1, start_col); 
    printf("Physical Memory:\n");
    for (int i = 0; i < frame_count; i++) {
        if (frame_table[i].valid) {
            printf("[P%d, Pg%d]", frame_table[i].process_id, frame_table[i].page_number);
        } else {
            printf("[Empty]");
        }
        if ((i + 1) % 5 == 0) {
            printf("\n");
            move_cursor((i + 1) / 5 + 2, start_col); // 다음 줄로 이동
        }
    }
    fflush(stdout);*/
}

void display_running_thread(Process *process, Thread *thread) {
    move_cursor(process_count * 5 + 1, 1);
    printf("Running Process %d's Thread %d\n", process->id, thread->tid);
    fflush(stdout);
}

void enqueue_process(Process **queue, Process *process) {
    if (*queue == NULL) {
        *queue = process;
    } else {
        Process *temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = process;
    }
    process->next = NULL;
}

Process *dequeue_process(Process **queue) {
    if (*queue == NULL) {
        return NULL;
    } else {
        Process *temp = *queue;
        *queue = (*queue)->next;
        temp->next = NULL;
        return temp;
    }
}

int get_queue_length(Process *queue) {
    int length = 0;
    while (queue != NULL) {
        length++;
        queue = queue->next;
    }
    return length;
}

Process *select_process() {
    int high_priority_count = get_queue_length(high_priority_queue);
    int mid_priority_count = get_queue_length(mid_priority_queue);
    int low_priority_count = get_queue_length(low_priority_queue);

    int total_weight = (high_priority_count * HIGH_PRIORITY_WEIGHT) +
                       (mid_priority_count * MID_PRIORITY_WEIGHT) +
                       (low_priority_count * LOW_PRIORITY_WEIGHT);

    if (total_weight == 0) {
        return NULL;
    }

    int random_value = rand() % total_weight;

    if (random_value < (high_priority_count * HIGH_PRIORITY_WEIGHT)) {
        return dequeue_process(&high_priority_queue);
    } else if (random_value < ((high_priority_count * HIGH_PRIORITY_WEIGHT) + (mid_priority_count * MID_PRIORITY_WEIGHT))) {
        return dequeue_process(&mid_priority_queue);
    } else {
        return dequeue_process(&low_priority_queue);
    }
}

void run_thread(Thread *thread, Process *process) {
    if (thread->remaining_time > 0) {
        int run_time = thread->remaining_time < TIME_QUANTUM ? thread->remaining_time : TIME_QUANTUM;
        for (int i = 0; i < run_time; i++) {
            sleep(1);
            thread->executed_time++;
            thread->remaining_time--;
            process->remaining_time--;
            add_total_cpu_time(1);
            increment_total_simulation_time();
            display_progress(); // Update display
        }

        if (thread->remaining_time == 0) {
            printf("Process %d's Thread %d completed.\n", process->id, thread->tid);
            if (process->remaining_time == 0) {
                process->state = COMPLETED;
                record_process_completion_time(process->id, total_simulation_time);
            }
        }
    }
}

void handle_io_interrupt(Process *process) {
    if (rand() % 100 < 30) { // 30% 확률로 I/O 인터럽트 발생
        process->state = WAITING;
        process->io_remaining_time = (rand() % 10) + 7; // I/O 대기 시간 설정
        process->was_interrupted = 1; // 인터럽트 발생 여부를 저장
        printf("Process %d I/O interrupt. Moving to I/O wait.\n", process->id);
        display_progress(); // Display progress after I/O interrupt
    }
}

void update_io_waiting_processes() {
    for (int i = 0; i < process_count; i++) {
        Process *process = processes[i];
        if (process->state == WAITING && process->io_remaining_time > 0) {
            process->io_remaining_time--; // 1초씩 감소시킴
            if (process->io_remaining_time == 0) {
                process->state = READY;
                process->was_interrupted = 0; // 인터럽트 플래그 초기화
                printf("Process %d I/O wait completed. Returning to ready state.\n", process->id);
                display_progress(); // Display progress after I/O wait completion
                // 프로세스를 다시 큐에 추가
                if (process->priority == 0) {
                    enqueue_process(&high_priority_queue, process);
                } else if (process->priority == 1) {
                    enqueue_process(&mid_priority_queue, process);
                } else {
                    enqueue_process(&low_priority_queue, process);
                }
            }
        }
    }
}

void multi_level_queue_scheduling() {
    // 프로세스를 각 우선순위 큐에 배치
    for (int i = 0; i < process_count; i++) {
        Process *process = processes[i];
        if (process->state == READY) {
            if (process->priority == 0) {
                enqueue_process(&high_priority_queue, process);
            } else if (process->priority == 1) {
                enqueue_process(&mid_priority_queue, process);
            } else {
                enqueue_process(&low_priority_queue, process);
            }
        }
    }

    Process *current_process;
    while (1) {
        current_process = select_process();
        while (current_process != NULL) {
            Thread *current_thread = current_process->threads;
            while (current_thread != NULL) {
                int page_number = current_thread->executed_time / TIME_QUANTUM;
                int frame_number = request_page(current_process, page_number); // 페이지 요청

                display_running_thread(current_process, current_thread);
                run_thread(current_thread, current_process);
                handle_io_interrupt(current_process); // I/O 인터럽트 처리

                if (current_thread->remaining_time == 0 || current_process->state == WAITING) {
                    break;
                }
                current_thread = current_thread->next;
            }

            update_io_waiting_processes(); // I/O 대기 중인 프로세스 상태 업데이트

            // 스케줄링 반복을 위해 큐 재배치 로직 이동
            if (current_process->state == READY) {
                if (current_process->priority == 0) {
                    enqueue_process(&high_priority_queue, current_process);
                } else if (current_process->priority == 1) {
                    enqueue_process(&mid_priority_queue, current_process);
                } else {
                    enqueue_process(&low_priority_queue, current_process);
                }
            }
            current_process = select_process();
        }

        // 모든 프로세스가 완료되었는지 확인
        int all_completed = 1;
        for (int i = 0; i < process_count; i++) {
            if (processes[i]->state != COMPLETED) {
                all_completed = 0;
                break;
            }
        }

        // 모든 프로세스가 완료되었으면 스케줄링 종료
        if (all_completed) {
            break;
        }

        // 아직 완료되지 않은 프로세스가 있는 경우 대기
        sleep(1);
        update_io_waiting_processes(); // I/O 대기 중인 프로세스 상태 업데이트
    }

    // Free processes after scheduling
    free_processes();
}
