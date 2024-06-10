#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SIZE 100


// 프로세스를 나타내는 구조체
typedef struct {
    int id;           // 프로세스 ID
    int priority;       // 프로세스의 우선순위
    int remaining;    // 프로세스의 남은 실행 시간
    int waiting_time; // 프로세스의 대기 시간
    bool interrupt; //interrupt 처리 유무
    int interrupttime; // interrupt하는 중 실행하는 time
    } Thread;

// Structure to represent the queue
typedef struct {
    Thread arr[MAX_SIZE];
    int front;
    int rear;
} Queue;

// Function to initialize the queue
void initializeQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
int isFull(Queue *queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

// Function to enqueue an element into the queue
void enqueue(Queue *queue, Thread value) {
    if (isFull(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }
    queue->arr[queue->rear] = value;
}

// Function to dequeue an element from the queue
Thread dequeue(Queue *queue) {
    Thread value;
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        exit(1);
    }
    value = queue->arr[queue->front];
    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }
    return value;
}

// Function to display the elements of the queue
void displayQueue(Queue *queue) {
    int i;
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue elements: ");
    for (i = queue->front; i != queue->rear; i = (i + 1) % MAX_SIZE) {
        printf("%d ", queue->arr[i].id);
    }
    printf("%d\n", queue->arr[i].id);
}

int size(Queue *queue) {
    if (isEmpty(queue)) {
        return 0;
    }
    return (queue->rear - queue->front + MAX_SIZE) % MAX_SIZE + 1;
}


void updateRemaintime(Queue *queue, int time)
{
    queue->arr[queue->front].remaining=queue->arr[queue->front].remaining-time;
}

/*front Thread 반환*/
Thread getFront(Queue *queue)
{

    return queue->arr[queue->front];
}

void updateWaitingtime(Queue *queue, int time)
{
    int i;
    for (i = queue->front; i != queue->rear; i = (i + 1) % MAX_SIZE) {
        queue->arr[i].waiting_time=queue->arr[i].waiting_time+time;
    }
    queue->arr[i].waiting_time=queue->arr[i].waiting_time+time;

}

void updateWaitingtime_byNext(Queue *queue, int time)
{
    int i;
    for (i = queue->front+1; i != queue->rear; i = (i + 1) % MAX_SIZE) {
        queue->arr[i].waiting_time=queue->arr[i].waiting_time+time;
    }
    queue->arr[i].waiting_time=queue->arr[i].waiting_time+time;

}

// Function to display the elements of the queue
void displayQueue_and_waiting(Queue *queue) {
    int i;
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    for (i = queue->front; i != queue->rear; i = (i + 1) % MAX_SIZE) {
        printf("(%d, ", queue->arr[i].id);
        printf("%d, ", queue->arr[i].remaining);
        printf("%d), ", queue->arr[i].waiting_time);
    }
    printf("(%d, ", queue->arr[i].id);
    printf("%d, ", queue->arr[i].remaining);
    printf("%d) \n\n", queue->arr[i].waiting_time);
}

void interrupt_Waitingtime(Queue *queue, int time)
{
    int i;
    for (i = queue->front; i != queue->rear; i = (i + 1) % MAX_SIZE) {
        queue->arr[i].interrupttime=queue->arr[i].interrupttime-time;
    }
    queue->arr[i].interrupttime=queue->arr[i].interrupttime-time;

}

void displayQueue_and_interrupt(Queue *queue) {
    int i;
    if (isEmpty(queue)) {
        printf("Wait Queue is empty.\n\n");
        return;
    }
    printf("Wait Queue(id, waiting_time, interrupt_time): ");
    for (i = queue->front; i != queue->rear; i = (i + 1) % MAX_SIZE) {
        printf("(%d, ", queue->arr[i].id);
        printf("%d, ", queue->arr[i].waiting_time);
        printf("%d), ", queue->arr[i].interrupttime);
    }
    printf("(%d, ", queue->arr[i].id);
    printf("%d, ", queue->arr[i].waiting_time);
    printf("%d) \n", queue->arr[i].interrupttime);
}
