#include "thread.h"

thread_t *current_thread = NULL;
thread_t *thread_queue = NULL;

void thread_wrapper(){
    if(current_thread && current_thread->start_routine){
        current_thread->start_routine(current_thread->arg);
    }
    //thread_t *temp_thread = current_thread;
    free(current_thread->stack);
    //free(current_thread->stack);
    current_thread = NULL;
    //free(temp_thread->stack);
    schedule();
}

int create_thread(thread_t *thread, void (*start_routine)(void *), void *arg) {
    thread->stack =  malloc(STACK_SIZE);
    if(!thread->stack) {
        return -1;
    }

    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->next = NULL;

    if(setjmp(thread->context) == 0 ){
        unsigned long *sp = (unsigned long*)((char*)thread->stack + STACK_SIZE - sizeof(unsigned long));
        *sp = (unsigned long)thread_wrapper;
       // sp -= 1;
#if defined(__x86_64__)
	thread->context[0].__jmpbuf[6] = (long)sp;
#elif defined(__i386__)
	thread->context[0].__jmpbuf[4] = (long)sp;
#elif defined(__aarch64__)
        thread->context[0].__jmpbuf[13] = (long)sp;
#else
	#error "error"
#endif

        if(!thread_queue) {
            thread_queue = thread;
        } else {
            thread_t *temp = thread_queue;
            while(temp->next){
                temp = temp->next;
            }
            temp->next = thread;
        }
    }
    return 0;
}

void schedule() {
	printf("current_thread: %p\n", current_thread);
    if(current_thread) {
        if(setjmp(current_thread->context) != 0) {
            //free(current_thread->stack);
            //current_thread =NULL;
	    printf("context !=0\n");
            return;
        }
    }
	printf("thread_queue: %p\n", thread_queue);
    if(thread_queue) {
        thread_t *previous_thread = current_thread;
        current_thread = thread_queue;
        thread_queue = thread_queue->next;

        if(previous_thread != current_thread) {
            longjmp(current_thread->context, 1);
        }
    }
}

