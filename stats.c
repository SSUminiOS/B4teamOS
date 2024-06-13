#include "stats.h"
#include <stdio.h>

int total_cpu_time = 0;
int total_simulation_time = 0;
int total_page_faults = 0;
int total_page_replacements = 0;
int total_page_requests = 0;
int total_page_hits = 0;

extern Process *processes[MAX_PROCESSES];
extern int process_count;

void initialize_stats() {
    total_cpu_time = 0;
    total_simulation_time = 0;
    total_page_faults = 0;
    total_page_replacements = 0;
    total_page_requests = 0;
    total_page_hits = 0;
}

void add_total_cpu_time(int time) {
    total_cpu_time += time;
}

void increment_total_simulation_time() {
    total_simulation_time++;
}

void record_page_fault() {
    total_page_faults++;
}

void record_page_replacement() {
    total_page_replacements++;
}

void record_page_request() {
    total_page_requests++;
}

void record_page_hit() {
    total_page_hits++;
}

void record_process_completion_time(int process_id, int completion_time) {
    processes[process_id - 1]->completion_time = completion_time;
}

void display_statistics() {
    printf("Total Page Faults: %d\n", total_page_faults);
    printf("Total Page Replacements: %d\n", total_page_replacements);
    if (total_page_requests > 0) {
        printf("Page Hit Rate: %.2f%%\n", ((double)total_page_hits / total_page_requests) * 100);
    } else {
        printf("Page Hit Rate: N/A\n");
    }
    for (int i = 0; i < process_count; i++) {
        printf("Process %d Completion Time: %d\n", processes[i]->id, processes[i]->completion_time);
    }
    if (total_simulation_time > 0) {
        printf("CPU Utilization: %.2f%%\n", ((double)total_cpu_time / total_simulation_time) * 100);
    } else {
        printf("CPU Utilization: N/A\n");
    }
}
