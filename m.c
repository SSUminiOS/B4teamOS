#include "thread.h"

void thread_function(void *arg) {
    printf("Thread %s: started\n", (char *)arg);
    for(int i = 0; i < 5; i++) {
        printf("Thread %s: %d\n", (char *)arg, i);
        //schedule();
    }
    printf("Thread %s: finished\n", (char *)arg);
}

int main() {
    my_thread_t thread1, thread2;

    if(create_thread(&thread1, thread_function, "1") == -1){
        perror("Failed to creat thread1");
        return 1;
    }
    
    if(create_thread(&thread2, thread_function, "2") == -1){
        perror("failed to create thread2");
        return 1;
    }

    printf("Main thread starting\n");

    schedule();
    
    printf("Main thread ending\n");
    return 0;

}
