#include <Artifex/engine.h>
#include <malloc.h>
#include <pthread.h>

#define __AX_SCHEDULER_IMPLEMENTATION__
#include "module.c"
#include "scheduler.c"

// Thread Backend
struct _ax_thread {
    // Control Parameters
    pthread_t p;
    volatile int done, *gdone;

    // Schedule
    uint8_t id;
    struct _ax_scheduler_thread* schedule;
};

// Engine Backend
struct _Artifex {
    // Thread List
    uint16_t thread_count;
    struct _ax_thread* threads;
    volatile int done;

    // Module Scheduler
    struct _ax_scheduler scheduler;

    // Main Thread's Modules (linked list)
    struct _ax_module* modules;
};

// Thread Process
void* _ax_thread_process(void*);

// Check if Engine Struct is in-tact
int _ax_is_ok(Artifex ax) {
    // TODO
    return 1;
}

int axInitialize(Artifex* ax, uint8_t threads) {
    if (!ax) {
        // TODO raise exception
        return AX_INVALID_INPUT;
    }

    // Allocate Engine
    if (!(*ax = malloc(sizeof(struct _Artifex)))) {
        // TODO raise exception
        return AX_INSUFFICENT_MEMORY;
    }

    // Allocate Threads
    threads = threads == 0 ? /* get cpu thread c */ 4 : threads;
    (*ax)->thread_count = threads;
    (*ax)->threads = threads == 0 ? NULL : malloc(sizeof(struct _ax_thread) * (*ax)->thread_count);

    if (!(*ax)->threads) {
        // TODO
        free(*ax);
        *ax = NULL;
        return AX_INSUFFICENT_MEMORY;
    }

    // Initialize Scheduler
    int err = _ax_shceduler_init(&(*ax)->scheduler, threads);
    if (err != AX_OK) {
        // TODO raise exception
        free((*ax)->threads);
        free(*ax);
        *ax = NULL;
        return err;
    }

    // Start Threads
    for (uint8_t i = 0; i < (*ax)->thread_count; i++) {
        struct _ax_thread* th = &(*ax)->threads[i];
        th->done = 1;  // make thread wait for update (TODO)
        th->gdone = &(*ax)->done;

        th->id = 0;  // TODO generate ID
        th->schedule = &(*ax)->scheduler.thread[i];

        th->queue_size = 0;
        th->queue = NULL;

        pthread_create(&th->p, NULL, _ax_thread_process, th);
    }

    // TODO

    return 0;
}

void axDestroy(Artifex* ax) {
    if (!ax || !_ax_is_ok(*ax)) {
        return;
    }

    // TODO

    return;
}

int axUpdate(Artifex ax) {
    if (!_ax_is_ok(ax)) {
        return 1;
    }

    ax->done = 0;
    for (int i = 0; i < ax->thread_count; i++)
        ax->threads[i].done = 0;

    // TODO run modules...

    while (ax->done < ax->thread_count);

    // TODO run late stuff

    return 0;
}

void axStartLoop(Artifex ax) {
    printf("Load Balance:\n");
    for (int i = 0; i < ax->thread_count; i++) {
        printf("thread: %i | processes: %i\n", i, ax->threads[i].module_count);
    }

    while (!axUpdate(ax)) {
        // for (uint32_t i = 0; i < ax->thread_count; ax++)
        // ax->threads[i].done = 0;
        // TODO main thread loop
    }
}

id_t axRegister(Artifex ax, const struct axModuleDescriptor* descriptor) {
    if (!_ax_is_ok(ax))
        return 1;
    static uint64_t cursor = 0;
    // TODO dependency check

    // Create Struct for new Module
    struct _ax_module* new = malloc(sizeof(struct _ax_module));
    new->user = descriptor->user;

    new->onCreate = descriptor->onCreate;
    new->onDestroy = descriptor->onDestroy;
    new->onUpdate = descriptor->onUpdate;
    new->id = cursor;
    cursor++;

    new->next = NULL;

    // Handle single-thread case
    if (ax->thread_count == 0)
        return _ax_module_append(&ax->modules, new);

    // Get best thread
    int best = -1;
    uint32_t value = 0;

    for (uint32_t i = 0; i < ax->thread_count; i++) {
        if (ax->threads[i].avg_time < value)
            best = i, value = ax->threads[i].avg_time;
    }

    if (best == -1) {
        best = 0;
        value = ax->threads[0].module_count;
        for (uint32_t i = 0; i < ax->thread_count; i++) {
            if (ax->threads[i].module_count < value)
                best = i, value = ax->threads[i].module_count;
        }
    }

    // Append Module List
    ax->threads[best].module_count++;
    return _ax_module_append(&ax->threads[best].modules, new);
}

int axEnable(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return 1;
    }

    // TODO

    return 0;
}

int axEnableTree(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return 1;
    }

    // TODO

    return 0;
}

int axDisable(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return 1;
    }

    // TODO

    return 0;
}

int axDisableTree(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return 1;
    }

    // TODO

    return 0;
}

void* _ax_thread_process(void* arg) {
    struct _ax_thread* th = arg;

    if (!th)
        return NULL;

    struct _ax_scheduler_thread* sch = th->schedule;

    if (!sch)
        return NULL;

    while (1) {
        while (th->done);

        // Update Tasks
        for (uint64_t i = 0; i < sch->task_length; i++) {
            // TODO update task i
        }

        // Update Queue Items
        for (uint64_t i = 0; i < sch->queue_size; i++) {
            // TODO update queue item i
        }

        // Iterate Modules
        struct _ax_module* module = th->modules;
        while (module != NULL) {
            if (module->onUpdate)
                module->onUpdate(module->id, module->user);
            // TODO update module
            module = module->next;
        }

        th->done = 1;
        (*th->gdone)++;

        // TODO create & update modules, time it
    }

    // TODO destroy modules

    return NULL;
}