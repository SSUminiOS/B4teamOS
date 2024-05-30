#include <stdio.h>
#include "queue.h"
#include <time.h>

#define MAX_PROCESSES 10
#define HIGH_PRIORITY_WEIGHT 7
#define MEDIUM_PRIORITY_WEIGHT 5
#define LOW_PRIORITY_WEIGHT 3

Queue queue_high, queue_medium, queue_low;

int total_time = 0;


void addProcessToQueue(Process process, Queue *queue_high, Queue *queue_medium, Queue *queue_low)
{
    if (process.priority == 1) {
        enqueue(queue_high, process);
    } else if (process.priority == 2) {
        enqueue(queue_medium, process);
    } 
    else 
    {
        enqueue(queue_low, process);
    }
}

void Process_Processing(Queue* q, int weight, int* count, int* i, int* context, int total_count, int* wait_time)
{
    int rest_weight = weight;

    while(rest_weight > 0)
    {
        Process value = getFront(q);
        if(value.remaining - rest_weight <= 0)//주어진 가중치보다 remaining이 더 작거나 같은 경우
        {
            Process q_value = dequeue(q);
            printf("Process %d from queue, and its waiting time is %d.\n", q_value.id, value.waiting_time);
            *i= q_value.remaining;
            *wait_time= *wait_time+*i;
            rest_weight = rest_weight - q_value.remaining;
            printf("rest_weight: %d \n", rest_weight);
            *context = *context+1;
            *count = size(q);
            total_count--;
            total_time = total_time + q_value.remaining*total_count;//total_time = total_time + 남은 remaining * 처리할 프로세스(스레드)의 갯수
            updateWaitingtime(q, *i);
            if(*count == 0)//현  queue가 비어있으면 while문을 빠져나감
            {
                printf("남은 프로세스 총 갯수: %d \n", total_count);
                updateWaitingtime(q, *i);
                displayQueue_and_waiting(q);
                printf("\n\n");
                printf("queue is empty\n\n");
                break;
            }
        }
        else //주어진 가중치보다 remaining이 더 클 경우
        {
            updateRemaintime(q, rest_weight);
            Process value = getFront(q);
            printf("Process %d from high queue, and its waiting time is %d, and still continue\n", value.id, value.waiting_time);
            printf("value current remaining: %d \n", value.remaining);
            *i= rest_weight;
            *wait_time+=*i;
            updateWaitingtime_byNext(q, *i);
            rest_weight = 0;
            total_time = total_time+rest_weight*total_count;//total_time = total_time + 남은 weight * 처리한 프로세스(스레드)의 갯수
            *context = *context+1;
        }
        printf("남은 프로세스 총 갯수: %d \n", total_count);
        
    }
                    
}

int customScheduling(Process processes[], int n) 
{
    
    int i = 0;
    int context = 0;
    int wtime;

    int rest_weight;

    int high_weight = HIGH_PRIORITY_WEIGHT;
    int medium_weight = MEDIUM_PRIORITY_WEIGHT;
    int low_weight = LOW_PRIORITY_WEIGHT;

    int high_count = size(&queue_high);
    int medium_count = size(&queue_medium);
    int low_count = size(&queue_low);

    int total_count = high_count + medium_count + low_count;

    int high_weighted = high_count * high_weight;
    int medium_weighted = medium_count * medium_weight;
    int low_weighted = low_count * low_weight;

    int total_weighted = high_weighted + medium_weighted + low_weighted;
    
    /*Process(thread)가 다 처리가 될 때까지*/
    while(total_count > 0)
    {
        wtime = 0;
        //total_weighted == 0일 경우 종료
        if(total_weighted == 0)
        {
            printf("Scheduling terminate\n");
            break;
        }

        srand(time(NULL));
        int random = rand() % total_weighted;

        /*high qeueue 처리*/
        if (random < high_weighted && high_count > 0) 
        {
            Process_Processing(&queue_high, high_weight, &high_count, &i, &context, total_count, &wtime);
            updateWaitingtime(&queue_medium, wtime);
            updateWaitingtime(&queue_low, wtime);
            displayQueue_and_waiting(&queue_high);
            displayQueue_and_waiting(&queue_medium);
            displayQueue_and_waiting(&queue_low);

            while(size(&queue_medium)>0)
            {
                Process m_value = getFront(&queue_medium);
                if(m_value.waiting_time > 20)
                {
                    Process d_m_value = dequeue(&queue_medium);
                    enqueue(&queue_high, d_m_value);
                    
                }
                else
                {
                    break;
                }
                
            }

            while(size(&queue_low)>0)
            {
                Process l_value = getFront(&queue_low);
                if(l_value.waiting_time > 20)
                {
                    Process d_l_value = dequeue(&queue_low);
                    enqueue(&queue_high, d_l_value);
                    
                }
                else if(l_value.waiting_time > 10)
                {
                    Process d_l_value = dequeue(&queue_low);
                    enqueue(&queue_medium, d_l_value);
                    
                }
                else
                {
                    break;
                }
                
            }
                
           
        } 

        /*medium qeueue 처리*/
        else if (random < high_weighted + medium_weighted && medium_count > 0) 
        {
            Process_Processing(&queue_medium, medium_weight, &medium_count, &i, &context, total_count, &wtime);
            updateWaitingtime(&queue_high, wtime);
            updateWaitingtime(&queue_low, wtime);

            displayQueue_and_waiting(&queue_high);
            displayQueue_and_waiting(&queue_medium);
            displayQueue_and_waiting(&queue_low);

            while(size(&queue_low)> 0)
            {
                Process l_value = getFront(&queue_low);
                if(l_value.waiting_time > 20)
                {
                    Process d_l_value = dequeue(&queue_low);
                    enqueue(&queue_high, d_l_value);
                    
                }
                else if(l_value.waiting_time > 10)
                {
                    Process d_l_value = dequeue(&queue_low);
                    enqueue(&queue_medium, d_l_value);
                    
                }
                else
                {
                    break;
                }
                
            }

                
        } 

        /*low qeueue 처리*/
        else if(random < total_weighted && low_count > 0)
        {
            Process_Processing(&queue_low, low_weight, &low_count, &i, &context, total_count, &wtime);
            updateWaitingtime(&queue_high, wtime);
            updateWaitingtime(&queue_medium, wtime);
            displayQueue_and_waiting(&queue_high);
            displayQueue_and_waiting(&queue_medium);
            displayQueue_and_waiting(&queue_low);

            while(size(&queue_medium)>0)
            {
                Process m_value = getFront(&queue_medium);
                if(m_value.waiting_time > 20)
                {
                    Process d_m_value = dequeue(&queue_medium);
                    enqueue(&queue_high, d_m_value);
                    
                }
                else
                {
                    break;
                }
                
            }

        }

        high_count = size(&queue_high);
        medium_count = size(&queue_medium);
        low_count = size(&queue_low);
        total_count = high_count + medium_count + low_count;

        high_weighted = high_count * high_weight;
        medium_weighted = medium_count * medium_weight;
        low_weighted = low_count * low_weight;

        total_weighted = high_weighted + medium_weighted + low_weighted;
        displayQueue(&queue_high);
        displayQueue(&queue_medium);
        displayQueue(&queue_low);
    }
    
    
    return context;
}

int main() {

    Process processes[MAX_PROCESSES] = {
        {1, 1, 1, 0},
        
        {2, 2, 2, 0},
        
        {3, 3, 3, 0},
        
        {4, 1, 4, 0},
        
        {5, 2, 5, 0},
       
        {6, 3, 6, 0},
        
        {7, 1, 7, 0},
        
        {8, 2, 8, 0},
        
        {9, 3, 9, 0},
       
        {10, 1, 10, 0},
 

    };


    initializeQueue(&queue_high);
    initializeQueue(&queue_medium);
    initializeQueue(&queue_low);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        addProcessToQueue(processes[i], &queue_high, &queue_medium, &queue_low);
    }

    // 각각의 큐들을 출력
    displayQueue(&queue_high);
    displayQueue(&queue_medium);
    displayQueue(&queue_low);
    
    int context_switch;
    // 프로세스 스케쥴링
    context_switch = customScheduling(processes, MAX_PROCESSES);
    
    printf("mean total time: %d\n", total_time/10);//평균 대기시간
    printf("contextswitch time: %d\n", context_switch);//context switch 횟수

    return 0;
}
