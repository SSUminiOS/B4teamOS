#include <stdio.h>
#include "queue.h"
#include <time.h>


#define MAX_PROCESSES 10
#define HIGH_PRIORITY_WEIGHT 7
#define MEDIUM_PRIORITY_WEIGHT 5
#define LOW_PRIORITY_WEIGHT 3

/*각 프로세스(스레드)들을 순위를 보고 해당하는 우선순위 큐에 집어넣는 함수*/
void addProcessToQueue(Thread process, Queue *queue_high, Queue *queue_medium, Queue *queue_low);

/*대기 큐의 interrupt time이 다 처리되었을 경우(대기큐에서 기존 있었던 큐로 빠져나감)*/
void updateWaitQueues(Queue* wait_queue, Queue* queue_high, Queue* queue_medium, Queue* queue_low, int* total_count, int wait_time, int check_priority);

/*지정된 특정 큐에 있는 스레드 처리*/
void Thread_Processing(Queue* q, int weight, int* count, int* i, int* context, int total_count, int* wait_time);

/*너무 waiting time이 걸릴 경우 우선순위 증가*/
void UpdatePriority();

/*스케줄링 시작*/
int customScheduling(Thread processes[], int n);

/*스케줄링 예시*/
void sched_ex();
