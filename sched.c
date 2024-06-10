#include <stdio.h>
#include "queue.h"
#include <time.h>

#define MAX_PROCESSES 10
#define HIGH_PRIORITY_WEIGHT 7
#define MEDIUM_PRIORITY_WEIGHT 5
#define LOW_PRIORITY_WEIGHT 3

Queue queue_high, queue_medium, queue_low, queue_wait;

int total_time = 0;//모든 프로세스(스레드)의 대기시간을 저장하기 위한 변수

/*각 프로세스(스레드)들을 순위를 보고 해당하는 우선순위 큐에 집어넣는 함수*/
void addProcessToQueue(Thread process, Queue *queue_high, Queue *queue_medium, Queue *queue_low)
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

//대기 큐의 interrupt time이 다 처리되었을 경우(대기큐에서 기존 있었던 큐로 빠져나감)
void updateWaitQueues(Queue* wait_queue, Queue* queue_high, Queue* queue_medium, Queue* queue_low, int* total_count, int wait_time, int check_priority)
{
    while(size(&queue_wait)>0)
    {
        Thread w_value = getFront(wait_queue);
        if(w_value.interrupttime<=0)
        {
            Thread d_w_value = dequeue(wait_queue);

            //현재 처리하고 있는 큐와 대기큐에서 나온 프로세스(스레드)의 우선순위가 다를 경우
            //밑에서 Updatewaiitime을 처리하기에 해당 코드에 대한 대비
            if(d_w_value.priority != check_priority)
            {
                d_w_value.waiting_time= d_w_value.waiting_time-wait_time;
            }
            //printf("d_w_value(id, waiting_time, interrupt_time): (%d, %d, %d) \n", d_w_value.id, d_w_value.waiting_time, d_w_value.interrupttime);
            addProcessToQueue(d_w_value, queue_high, queue_medium, queue_low);
            total_count++;//queue에 값이 들어갔으므로 total_count 증가
        }
        else
        {

            break;
        }

    }
    displayQueue_and_interrupt(&queue_wait);
}


/*지정된 특정 큐에 있는 스레드 처리*/
void Thread_Processing(Queue* q, int weight, int* count, int* i, int* context, int total_count, int* wait_time)
{
    int rest_weight = weight;//현재 남은 weight 저장
    int check_priority = getFront(q).priority;//처리되고 있는 queue가 무슨 큐인지 저장(updateWaitQueues에 사용하기 위한 매개변수)

    while(rest_weight > 0 && size(q)>0)
    {
        Thread value = getFront(q);

        //interrupt 유뮤 확인(있으면 wait_queue에 넣고 아니면 Process(thread) scheduling 처리)
        if(value.interrupttime > 0)
        {
            printf("Thread %d is pushed in wait_queue\n\n", value.id);
            //weight_queue에 넣는 함수
            Thread wait_value = dequeue(q);
            enqueue(&queue_wait, wait_value);
            total_count--;
        }
        else
        {
            if(value.remaining - rest_weight <= 0)//주어진 가중치보다 remaining이 더 작거나 같은 경우
            {
                Thread q_value = dequeue(q);
                printf("Thread %d from queue, and its waiting time is %d.\n", q_value.id, value.waiting_time);
                *i= q_value.remaining;//reamining 저장
                *wait_time= *wait_time+*i;//현재 걸렸던 시간 저장
                rest_weight = rest_weight - q_value.remaining;//주어진 weight값 갱신
                printf("rest_weight: %d \n\n", rest_weight);

                *context = *context+1;//context_switch 추가

                *count = size(q);//dequeu 처리로 queue 안에 프로세스(스레드) 갯수 갱신

                total_count--;//총 프로세스(스레드)값을 줄임
                total_time = total_time + q_value.waiting_time;//total_time = total_time + 처리된 프로세스(스 레드)의 waiting_time
                updateWaitingtime(q, *i);//해당 큐에 다른 프로세스(스레드) 대기시간 갱신

                if(*count == 0)//현 queue가 비어있으면 while문을 빠져나감
                {
                    printf("남은 프로세스 총 갯수: %d \n", total_count);
                    updateWaitingtime(q, *i);
                    printf("queue is empty\n\n");

                    //대기큐의 값이 있으면 처리된 시간만큼 interrupt time 갱신
                    interrupt_Waitingtime(&queue_wait, *i);

                   //대기 큐의 interrupt time이 다 처리되었을 경우(대기큐에서 기존 있었던 큐로 빠져나감)
                   updateWaitQueues(&queue_wait, &queue_high, &queue_medium, &queue_low, &total_count, *wait_time, check_priority);
                   break;
                }
            }

            else //주어진 가중치보다 remaining이 더 클 경우
            {
                updateRemaintime(q, rest_weight);//reaminingtime 갱신
                Thread value = getFront(q);
                printf("Thread %d from queue, and its waiting time is %d, and still continue\n", value.id, value.waiting_time);
                printf("value current remaining: %d \n\n", value.remaining);
                *i= rest_weight;//남은 가중치 저장
                *wait_time+=*i;//현재 걸렸던 시간 저장

                if(size(q)!=1)//여기서는 지금 처리되는 프로세스의 대기시간은 갱신하면 안되기에 updateWaiting_byNext함수로 처리
                {
                    updateWaitingtime_byNext(q, *i);
                }
                rest_weight = 0;//더이상 주어진 가중치는 없음(0 처리)
                *context = *context+1;//context_switch 추가
            }

            //대기큐의 값이 있으면 처리된 시간만큼 interrupt time 갱신
            interrupt_Waitingtime(&queue_wait, *i);
            updateWaitQueues(&queue_wait, &queue_high, &queue_medium, &queue_low, &total_count, *wait_time, check_priority);

            printf("현재 남은 프로세스 총 갯수: %d \n\n", total_count);
        }

    }

}


/*너무 waiting time이 걸릴 경우 우선순위 증가*/
void UpdatePriority()
{
    //medium_queue 처리
    while(size(&queue_medium)>0)
    {
        Thread m_value = getFront(&queue_medium);
        if(m_value.waiting_time > 20)
        {
            Thread d_m_value = dequeue(&queue_medium);
            d_m_value.priority = 1;
            enqueue(&queue_high, d_m_value);

        }
        else
        {
            break;
        }

    }
    //low_queue 처리
    while(size(&queue_low)>0)
    {
        Thread l_value = getFront(&queue_low);
        if(l_value.waiting_time > 20)
        {
            Thread d_l_value = dequeue(&queue_low);
            d_l_value.priority = 1;
            enqueue(&queue_high, d_l_value);

        }
        else if(l_value.waiting_time > 10)
        {
            Thread d_l_value = dequeue(&queue_low);
            d_l_value.priority = 2;
            enqueue(&queue_medium, d_l_value);

        }
        else
        {
            break;
        }

    }

}

int customScheduling(Thread processes[], int n)
{

    int i=0;

    int context = 0;//context_switch 저장
    int wtime;////현재 queue에서 걸린 시간(updateWaitingtime 에 사용)

    //각 queue마다의 가중치 다시 선언
    int high_weight = HIGH_PRIORITY_WEIGHT;
    int medium_weight = MEDIUM_PRIORITY_WEIGHT;
    int low_weight = LOW_PRIORITY_WEIGHT;

    //초기 각 queue 사이즈
    int high_count = size(&queue_high);
    int medium_count = size(&queue_medium);
    int low_count = size(&queue_low);

    int total_count = high_count + medium_count + low_count;//총 사이즈

    //가중치*각 queue의 프로세스(스레드) 갯수
    int high_weighted = high_count * high_weight;
    int medium_weighted = medium_count * medium_weight;
    int low_weighted = low_count * low_weight;

    int total_weighted = high_weighted + medium_weighted + low_weighted;//총 합

    /*Process(thread)가 다 처리가 될 때까지*/
    while(total_count > 0)
    {
        wtime = 0;

        //total_weighted == 0일 경우 종료
        if(total_weighted == 0)
        {
            break;
        }

        srand(time(NULL));
        int random = rand() % total_weighted;

        /*high qeueue 처리*/
        if (random < high_weighted && high_count > 0)
        {
            printf("high접근\n");
            Thread_Processing(&queue_high, high_weight, &high_count, &i, &context, total_count, &wtime);

            // printf("high_queue: ");
            // displayQueue_and_waiting(&queue_high);
            // printf("medium_queue: ");
            // displayQueue_and_waiting(&queue_medium);
            // printf("low_queue: ");
            // displayQueue_and_waiting(&queue_low);

            updateWaitingtime(&queue_medium, wtime);
            updateWaitingtime(&queue_low, wtime);

            printf("high_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_high);
            printf("medium_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_medium);
            printf("low_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_low);

            UpdatePriority();


        }

        /*medium qeueue 처리*/
        else if (random < high_weighted + medium_weighted && medium_count > 0)
        {
            printf("medium접근\n");
            Thread_Processing(&queue_medium, medium_weight, &medium_count, &i, &context, total_count, &wtime);

            // printf("high_queue: ");
            // displayQueue_and_waiting(&queue_high);
            // printf("medium_queue: ");
            // displayQueue_and_waiting(&queue_medium);
            // printf("low_queue: ");
            // displayQueue_and_waiting(&queue_low);

            updateWaitingtime(&queue_high, wtime);
            updateWaitingtime(&queue_low, wtime);



            printf("high_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_high);
            printf("medium_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_medium);
            printf("low_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_low);

           UpdatePriority();


        }

        /*low qeueue 처리*/
        else if(random < total_weighted && low_count > 0)
        {
            printf("low접근\n");
            Thread_Processing(&queue_low, low_weight, &low_count, &i, &context, total_count, &wtime);

            // printf("high_queue: ");
            // displayQueue_and_waiting(&queue_high);
            // printf("medium_queue: ");
            // displayQueue_and_waiting(&queue_medium);
            // printf("low_queue: ");
            // displayQueue_and_waiting(&queue_low);

            updateWaitingtime(&queue_high, wtime);
            updateWaitingtime(&queue_medium, wtime);

            printf("high_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_high);
            printf("medium_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_medium);
            printf("low_queue(id, reamining_time, waiting_time): ");
            displayQueue_and_waiting(&queue_low);

           UpdatePriority();

        }

        high_count = size(&queue_high);
        medium_count = size(&queue_medium);
        low_count = size(&queue_low);
        total_count = high_count + medium_count + low_count;

        // displayQueue(&queue_high);
        // displayQueue(&queue_medium);
        // displayQueue(&queue_low);
        // printf("\n");
        // printf("high_count: %d \n",high_count);
        // printf("medium_count: %d \n",medium_count);
        // printf("low_count: %d \n",low_count);
        // printf("total_count: %d \n",total_count);
        // printf("wait_count: %d \n\n", size(&queue_wait));

        //각 high, medium, low 큐는 비어있지만 wait 큐는 있능 경우(interruptitme에 -10을 해서 다 빠져나오게 함)
        if(total_count == 0 && size(&queue_wait)>0)
        {
            interrupt_Waitingtime(&queue_wait, 10);
            while(size(&queue_wait)>0)
            {
                Thread w_value = getFront(&queue_wait);
                if(w_value.interrupttime<=0)
                {
                    Thread d_w_value = dequeue(&queue_wait);
                    addProcessToQueue(d_w_value, &queue_high, &queue_medium, &queue_low);
                    total_count++;
                    printf("wait가 남았을 때 total_count: %d \n\n",total_count);
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
        printf("\n");
        printf("high_count: %d \n",high_count);
        printf("medium_count: %d \n",medium_count);
        printf("low_count: %d \n",low_count);
        printf("total_count: %d \n",total_count);
        printf("wait_count: %d \n\n", size(&queue_wait));
    }

    printf("Scheduling terminate\n");
    return context;
}

int main(){

    Thread processes[MAX_PROCESSES] = {
        {1, 1, 1, 0, 1 ,10},

        {2, 2, 2, 0, 0, 0},

        {3, 3, 3, 0, 1, 5},

        {4, 1, 4, 0, 0, 0},

        {5, 2, 5, 0, 0, 0},

        {6, 3, 6, 0, 0, 0},

        {7, 1, 7, 0, 1, 8},

        {8, 2, 8, 0, 0, 0},

        {9, 3, 9, 0, 1, 6},

        {10, 1, 10, 0, 0, 0},


    };


    initializeQueue(&queue_high);
    initializeQueue(&queue_medium);
    initializeQueue(&queue_low);
    initializeQueue(&queue_wait);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        addProcessToQueue(processes[i], &queue_high, &queue_medium, &queue_low);
    }

    // 각각의 큐들을 출력
    printf("초기 상태\n");
    displayQueue(&queue_high);
    displayQueue(&queue_medium);
    displayQueue(&queue_low);
    printf("\n");

    int context_switch;

    // 프로세스 스케쥴링(여기서 context_switch횟수 return)
    context_switch = customScheduling(processes, MAX_PROCESSES);

    printf("mean total time: %d\n", total_time);//총 대기시간
    printf("contextswitch time: %d\n", context_switch);//context switch 횟수

    return 0;
}
