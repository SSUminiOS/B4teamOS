#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <string.h>

#define STACK_SIZE (1024 * 64)

typedef struct my_thread {
    void (*start_routine)(void *);
    void *arg;
    void *stack;
    struct my_thread *next;
    ucontext_t context;
} my_thread_t;

my_thread_t *current_thread = NULL;
my_thread_t *thread_queue = NULL;

void thread_wrapper();

int create_thread(my_thread_t *thread, void (*start_routine)(void *), void *arg){
    thread->stack = malloc(STACK_SIZE);
    if(!thread->stack){
        return -1;
    }

    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->next = NULL;

    if(getcontext(&thread->context) == -1) {
        free(thread->stack);
        return -1;
    }

    thread->context.uc_stack.ss_sp = thread->stack;
    thread->context.uc_stack.ss_size = STACK_SIZE;
    thread->context.uc_link = NULL;
    makecontext(&thread->context, thread_wrapper, 0);

    if(!thread_queue) {
        thread_queue = thread;
    } else {
        my_thread_t *temp = thread_queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = thread;
    }
    return 0;
}

void schedule() {
    if(current_thread) {
        my_thread_t *previous_thread = current_thread;
        current_thread = thread_queue;
        if(thread_queue){
            thread_queue = thread_queue->next;
        }

        if(previous_thread != current_thread){
            //my_thread_t *temp_thread = previous_thread;
            //previous_thread =NULL;
            //swapcontext(&temp_thread->context, &current_thread->context);
            if(swapcontext(&previous_thread->context, &current_thread->context) == -1) {
                perror("swapcontext");
                exit(EXIT_FAILURE);
            }
        }

    } else {
        if(thread_queue) {
            current_thread = thread_queue;
            thread_queue = thread_queue->next;
            if(setcontext(&current_thread->context) == -1) {
                perror("setcontext");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void thread_wrapper() {
    if(current_thread && current_thread->start_routine) {
        current_thread->start_routine(current_thread->arg);
    }
    free(current_thread->stack);
    current_thread = NULL;
    schedule();
}

#endif
