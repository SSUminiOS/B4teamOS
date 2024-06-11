#include "memory.h"
#include "stats.h"

FrameTableEntry *frame_table;
int frame_count = 32; // 프레임 수를 증가시킴
int current_time = 0;

void initialize_page_table(Process *process, int page_count) {
    process->page_table = (PageTableEntry *)malloc(sizeof(PageTableEntry) * page_count);
    process->page_count = page_count;
    for (int i = 0; i < page_count; i++) {
        process->page_table[i].page_number = i;
        process->page_table[i].frame_number = -1;
        process->page_table[i].valid = 0;
        process->page_table[i].referenced = 0;
        process->page_table[i].modified = 0;
    }
}

void load_page_from_disk(Process *process, int page_number, int frame_number) {
    // 단순한 디스크에서 메모리로의 페이지 로드 시뮬레이션
    printf("Loading page %d of process %d into frame %d\n", page_number, process->id, frame_number);
    process->page_table[page_number].frame_number = frame_number;
    process->page_table[page_number].valid = 1;
    frame_table[frame_number].page_number = page_number;
    frame_table[frame_number].process_id = process->id;
    frame_table[frame_number].valid = 1;
    frame_table[frame_number].referenced = current_time++; // LRU 구현을 위한 참조 시간 업데이트
}

int find_free_frame() {
    for (int i = 0; i < frame_count; i++) {
        if (frame_table[i].valid == 0) {
            return i;
        }
    }
    return -1; // 모든 프레임이 사용 중인 경우
}

int replace_page(Process *process, int page_number) {
    // LRU 페이지 교체 알고리즘
    int lru_frame = 0;
    for (int i = 1; i < frame_count; i++) {
        if (frame_table[i].referenced < frame_table[lru_frame].referenced) {
            lru_frame = i;
        }
    }
    int replaced_process_id = frame_table[lru_frame].process_id;
    int replaced_page_number = frame_table[lru_frame].page_number;

    // 페이지 테이블 업데이트
    Process *replaced_process = processes[replaced_process_id - 1];
    replaced_process->page_table[replaced_page_number].valid = 0;
    replaced_process->page_table[replaced_page_number].frame_number = -1;

    load_page_from_disk(process, page_number, lru_frame);
    record_page_replacement();
    return lru_frame;
}

void handle_page_fault(Process *process, int page_number) {
    printf("Page fault in process %d for page %d\n", process->id, page_number);
    record_page_fault();
    int free_frame = find_free_frame();
    if (free_frame == -1) {
        replace_page(process, page_number);
    } else {
        load_page_from_disk(process, page_number, free_frame);
    }
}

int request_page(Process *process, int page_number) {
    record_page_request();
    if (page_number >= process->page_count) {
        printf("Invalid page request for process %d: page %d\n", process->id, page_number);
        return -1;
    }
    if (process->page_table[page_number].valid == 0) {
        handle_page_fault(process, page_number);
    } else {
        printf("Page %d of process %d is already in memory (frame %d)\n", page_number, process->id, process->page_table[page_number].frame_number);
        frame_table[process->page_table[page_number].frame_number].referenced = current_time++; // LRU 구현을 위한 참조 시간 업데이트
        record_page_hit();
    }
    return process->page_table[page_number].frame_number;
}
