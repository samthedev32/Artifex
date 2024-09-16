#include <Artifex/error.h>

// #include <unistd.h> // not on windows
#include <malloc.h>
#include <stdint.h>

// Amount of allowed frame-overrun
#define OVERRUN_BALANCE 0

struct _ax_scheduler_thread {
    // Static Task List (Run Every Frame)
    uint64_t task_length;
    uint64_t* task_list;

    // Extra Task Queue
    uint64_t queue_size;
    uint64_t* queue;

    uint32_t avg_time;
};

// Backend Scheduler
struct _ax_scheduler {
    // Thread Data
    uint8_t count;
    struct _ax_scheduler_thread* thread;

    uint64_t tasks;
    uint64_t* task;  // list of tasks
};

// Initialize Scheduler
int _ax_shceduler_init(struct _ax_scheduler* s, uint8_t threads);

// Destroy Scheduler
void _ax_scheduler_destroy(struct _ax_scheduler* s);

// Update Scheduler (updates schedules)
void _ax_scheduler_update(struct _ax_scheduler* s);

// Register Process/Module
void _ax_scheduler_register(struct _ax_scheduler* s, uint64_t id);

// Implementation
#define __AX_SCHEDULER_IMPLEMENTATION__
#ifdef __AX_SCHEDULER_IMPLEMENTATION__

int _ax_shceduler_init(struct _ax_scheduler* s, uint8_t threads) {
    if (s == NULL || threads == 0)
        return AX_INVALID_INPUT;

    // Allocate Thread Memory
    s->count = threads;
    s->thread = calloc(threads, sizeof(struct _ax_scheduler_thread));

    if (!s->thread)
        return AX_INSUFFICENT_MEMORY;

    return AX_OK;
}

void _ax_scheduler_destroy(struct _ax_scheduler* s) {
    // TODO
}

void _ax_scheduler_update(struct _ax_scheduler* s) {
    // TODO
}

void _ax_scheduler_register(struct _ax_scheduler* s, uint64_t id) {
    // TODO
}

#endif