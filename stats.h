#ifndef STATS_H
#define STATS_H

#include "memory.h"

extern int total_cpu_time;
extern int total_simulation_time;
extern int total_page_faults;
extern int total_page_replacements;
extern int total_page_requests;
extern int total_page_hits;

extern Process *processes[MAX_PROCESSES];

void initialize_stats();
void add_total_cpu_time(int time);
void increment_total_simulation_time();
void record_page_fault();
void record_page_replacement();
void record_page_request();
void record_page_hit();
void record_process_completion_time(int process_id, int completion_time);
void display_statistics();

#endif // STATS_H
