#ifndef PROCESS_H
#define PROCESS_H

#include "memory.h"

extern Process *processes[MAX_PROCESSES];
extern int process_count;

void create_thread(Process *process, int tid, int burst_time);
void create_process(int id, int arrival_time, int burst_time, int page_count);
void free_processes();
extern void display_processes();

#endif // PROCESS_H
