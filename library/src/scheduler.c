#include "module.c"

// Amount of allowed frame-overrun
#define OVERRUN_BALANCE 0

// Scheduler Task
struct _ax_scheduler_task {
    uint64_t id;  // task's unique id

    // TODO
};

// Backend Scheduler
struct _ax_scheduler {
    uint8_t threads;
};

// Initialize Scheduler
int _ax_shceduler_init(struct _ax_scheduler* s, uint8_t threads);

// Destroy Scheduler
void _ax_scheduler_destroy(struct _ax_scheduler* s);

// Update Scheduler (updates schedules)
void _ax_scheduler_update(struct _ax_scheduler* s);

// Register Process/Module
void _ax_scheduler_register(struct _ax_scheduler* s, uint64_t id);