#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "memory.h"
#include "schedule.h"
#include "process.h"
#include "stats.h"

/*
// Function to display a frame box with process information
void display_frame_box(int pid, int page_count) {
    clear_screen();
    printf("  +------------------+\n");
    printf("  |  Process PID: %d  |\n", pid);
    printf("  |  Page Count: %d   |\n", page_count);
    printf("  +------------------+\n");
    sleep(3); // Display for 3 seconds
    clear_screen();
}
*/
int main() {
    srand(time(NULL)); // Initialize random seed

    // Initialize frame table
    frame_table = (FrameTableEntry *)malloc(sizeof(FrameTableEntry) * FRAME_COUNT);
    for (int i = 0; i < FRAME_COUNT; i++) {
        frame_table[i].frame_number = i;
        frame_table[i].valid = 0;
    }

    initialize_stats();

    int choice;

    while (1) {
        printf("\n1. Create Random Process\n2. Display Processes\n3. Schedule Processes (Multi-Level Queue with Random Selection)\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int id = process_count + 1;
                int arrival_time = rand() % 10;
                int burst_time = (rand() % MAX_BURST_TIME) + 1;
                int page_count = (burst_time / TIME_QUANTUM) + 1; // Number of required pages
                create_process(id, arrival_time, burst_time, page_count);
                // display_frame_box(id, page_count);
                break;
            }
            case 2:
                display_processes();
                break;
            case 3:
                multi_level_queue_scheduling();
                display_statistics();
                break;
            case 4:
                free_processes();
                free(frame_table);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
