#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_POINTS 10000000  // 전체 무작위 점의 수
#define NUM_THREADS 10       // 스레드의 수

struct point
{
    double x;
    double y;
};

int num[NUM_THREADS];//각 스레드 저장 배열
struct point points[NUM_POINTS];//각 점 마다의 x,y 값 저장 배열

void point_set();
void *cal_pi(void *);
bool cal_distance(double , double );
void *check_num(void *);



void monte_pie() {
    point_set();//점 생성

    pthread_t threads[NUM_THREADS];

    for (int i = 1; i <= NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, cal_pi, (void *) i); // 스레드 생성
    }

    for (int i = 1; i <= NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // 스레드가 종료될 때까지 기다림
    }


}

// 무작위 점을 생성
void point_set() 
{
    srand(time(NULL));  // 무작위 숫자 생성을 위한 시드 설정
    for (long int i = 0; i < NUM_POINTS; i++) {
        points[i].x = (double)rand() / RAND_MAX * 2 - 1; // -1에서 1 사이의 무작위 x 좌표 생성
        points[i].y = (double)rand() / RAND_MAX * 2 - 1; // -1에서 1 사이의 무작위 y 좌표 생성

    }
}

//pi 추정 및 출력
void *cal_pi(void *param)
{
    long int thread_id = (long int) param;
    pthread_t tid2;
    pthread_create(&tid2, NULL, check_num, (void *)thread_id);
    pthread_join(tid2, NULL);
    double estimated_pi = 4.0 * (num[thread_id]) / ((NUM_POINTS/NUM_THREADS)*thread_id);
    printf("The circle is %d and total is %ld\n", num[thread_id], ((NUM_POINTS/NUM_THREADS)*thread_id));
    printf("Pi is %f\n", estimated_pi);
    sleep(1);
    pthread_exit(0);


}

//점이 원 안에 있는지 판단
bool cal_distance(double x, double y)
{
    double diff = (x*x)+(y*y);
    if(diff <= 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}


//점이 원 안에 있으면 +1처리
void *check_num(void *param)
{
    long int thread_id = (long int)param;
    num[thread_id] = 0;
    for(long int i = 0; i < ((NUM_POINTS/NUM_THREADS)*thread_id); i++)
    {
        if(cal_distance(points[i].x, points[i].y))
        {
            num[thread_id] = num[thread_id]+1;
        }
    }
    pthread_exit(0);
}


