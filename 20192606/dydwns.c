#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 1024 * 64
#define MAX_THREADS 10

typedef struct ThreadControlBlock {
    int id;
    ucontext_t context;
    void (*start_routine)(void*);
    void *arg;
    struct ThreadControlBlock *next;
} TCB;

TCB *thread_list = NULL;
TCB *current_thread = NULL;
int thread_count = 0;

void thread_wrapper(void (*start_routine)(void*), void *arg) {
    start_routine(arg);
    // Thread finished, remove it from the list and switch to next thread
    TCB *prev_thread = current_thread;
    TCB *temp = thread_list;
    
    while (temp->next != current_thread) {
        temp = temp->next;
    }
    temp->next = current_thread->next;
    current_thread = current_thread->next;
    thread_count--;

    free(prev_thread);
    setcontext(&current_thread->context);
}

int create_thread(void (*start_routine)(void*), void *arg) {
    if (thread_count >= MAX_THREADS) {
        return -1;
    }
    
    TCB *new_thread = (TCB*)malloc(sizeof(TCB));
    if (!new_thread) {
        return -1;
    }

    new_thread->id = thread_count++;
    new_thread->start_routine = start_routine;
    new_thread->arg = arg;

    if (getcontext(&new_thread->context) == -1) {
        free(new_thread);
        return -1;
    }

    new_thread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread->context.uc_stack.ss_size = STACK_SIZE;
    new_thread->context.uc_link = NULL;

    makecontext(&new_thread->context, (void (*)(void))thread_wrapper, 2, start_routine, arg);

    if (thread_list == NULL) {
        thread_list = new_thread;
        new_thread->next = new_thread;
    } else {
        TCB *temp = thread_list;
        while (temp->next != thread_list) {
            temp = temp->next;
        }
        temp->next = new_thread;
        new_thread->next = thread_list;
    }
    
    return new_thread->id;
}

void schedule() {
    if (thread_count == 0) {
        printf("No threads to schedule.\n");
        return;
    }

    if (current_thread == NULL) {
        current_thread = thread_list;
    } else {
        current_thread = current_thread->next;
    }

    setcontext(&current_thread->context);
}

void yield() {
    if (thread_count > 1) {
        TCB *previous_thread = current_thread;
        current_thread = current_thread->next;
        swapcontext(&previous_thread->context, &current_thread->context);
    }
}

void thread_function(void *arg) {
    int *id = (int*)arg;
    for (int i = 0; i < 5; i++) {
        printf("Thread %d iteration %d\n", *id, i);
        yield();
    }
}

int main() {
    int thread_ids[MAX_THREADS];
    for (int i = 0; i < 3; i++) {
        thread_ids[i] = i;
        create_thread(thread_function, &thread_ids[i]);
    }

    while (thread_count > 0) {
        schedule();
    }

    return 0;
}
