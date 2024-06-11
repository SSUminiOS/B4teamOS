#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10
#define MAX_BURST_TIME 20
#define TIME_QUANTUM 2
#define FRAME_COUNT 32

typedef struct Thread {
    int tid;
    int burst_time;
    int remaining_time;
    int executed_time; // 현재까지 실행된 시간
    struct Thread *next;
} Thread;

typedef struct PageTableEntry {
    int page_number;
    int frame_number;
    int valid;       // 1 if the page is in memory, 0 otherwise
    int referenced;  // Used for page replacement algorithms
    int modified;    // 1 if the page has been modified, 0 otherwise
} PageTableEntry;

typedef enum {
    READY,
    RUNNING,
    WAITING,
    COMPLETED // 완료 상태 추가
} ProcessState;

typedef struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    Thread *threads;
    PageTableEntry *page_table;
    int page_count;
    int completion_time; // 완료 시간 추가
    int io_remaining_time; // I/O 대기 시간
    ProcessState state; // 프로세스 상태
    struct Process *next; // 큐에서의 다음 프로세스를 가리키는 포인터
    int priority; // 프로세스 우선순위
    int was_interrupted;
} Process;

typedef struct FrameTableEntry {
    int frame_number;
    int page_number;
    int process_id;
    int valid;
    int referenced;  // 추가된 필드: LRU 알고리즘을 위한 참조 시간
} FrameTableEntry;

extern Process *processes[MAX_PROCESSES];
extern int process_count;
extern FrameTableEntry *frame_table;
extern int frame_count;

void initialize_page_table(Process *process, int page_count);
void create_thread(Process *process, int tid, int burst_time);
void create_process(int id, int arrival_time, int burst_time, int page_count);
void load_page_from_disk(Process *process, int page_number, int frame_number);
int find_free_frame();
int replace_page(Process *process, int page_number);
void handle_page_fault(Process *process, int page_number);
int request_page(Process *process, int page_number);
void free_processes();

#endif // MEMORY_H
