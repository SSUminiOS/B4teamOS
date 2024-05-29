#include "thread.h"
#include <stdlib.h>
#include <stdio.h>
void thread_function(void *arg){
    printf("Thread %s starting\n", (char *)arg);
    for(int i=0; i< 5; i++) {
        printf("Thread %s: %d\n", (char *)arg, i);
        schedule();
    }
    printf("Thread %s ending\n", (char *)arg);
}

int main() {
    thread_t thread1, thread2;
    thread_t main_thread;

    main_thread.next = NULL;

    current_thread = &main_thread;

    create_thread(&thread1, thread_function, "1");
    create_thread(&thread2, thread_function, "2");

    printf("Main thread starting\n");

    schedule();

    printf("Main thread ending\n");

    return 0;
}
