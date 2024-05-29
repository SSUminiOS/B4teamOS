#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#define STACK_SIZE (1024 * 1024)

typedef struct thread {
    void (*start_routine)(void *);
    void *arg;
    void *stack;
    struct thread *next;
    jmp_buf context;
} thread_t;

extern thread_t *current_thread;// = NULL;
extern thread_t *thread_queue;// = NULL;

void thread_wrapper();

int create_thread(thread_t *thread, void (*start_routine)(void *), void *arg);

void schedule();
/*
    thread->stack = malloc(STACK_SIZE);
    if (!thread->stack) {
        return -1;
    }

    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->next = NULL;

    if (setjmp(thread->context) == 0) {
        // 스택 포인터 설정
        unsigned long *sp = (unsigned long *)((char *)thread->stack + STACK_SIZE-sizeof(unsigned long));
        //sp -= 2;
        // 스택의 맨 위에 thread_wrapper 함수의 주소를 저장
        *sp = (unsigned long)thread_wrapper;
        //thread->context[0].__jmpbuf[6] = (long)sp;

#ifdef __x86_64__
        thread->context[0].__jmpbuf[6] = (long)sp;  // x86_64 아키텍처에서는 6번 인덱스가 SP를 가리킴
#elif defined(__i386__)
        thread->context[0].__jmpbuf[4] = (long)sp;  // i386 아키텍처에서는 4번 인덱스가 SP를 가리킴
#elif defined(__aarch64__)
        thread->context[0].__jmpbuf[13] = (long)sp;  // aarch64 아키텍처에서는 13번 인덱스가 SP를 가리킴
#else
//#error "Unsupported architecture"
#endif

        if (!thread_queue) {
            thread_queue = thread;
        } else {
            thread_t *temp = thread_queue;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = thread;
        }
    }

    return 0;
}

void schedule() {
    if (current_thread) {
        if (setjmp(current_thread->context) != 0) {
            return;
        }
    }

    if (thread_queue) {
        thread_t *previous_thread = current_thread;
        current_thread = thread_queue;
        thread_queue = thread_queue->next;

        if (previous_thread != current_thread) {
            longjmp(current_thread->context, 1);
        }
    }
}

void thread_wrapper() {
    if (current_thread && current_thread->start_routine) {
        current_thread->start_routine(current_thread->arg);
    }
    free(current_thread->stack);
    current_thread = NULL;
    schedule();
}
*/

#endif
