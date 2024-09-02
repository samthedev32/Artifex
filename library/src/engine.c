#include <Artifex/engine.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>

struct _ax_module {
    int enabled;
    void* user;

    // TODO callbacks

    struct _ax_module* next;  // next module(s)
};

struct _ax_thread {
    pthread_t p;

    // module linked-list
    uint16_t module_count;
    struct _ax_module* modules;

    uint32_t avg_time;
    volatile int done;
};

struct _Artifex {
    // thread list
    uint16_t thread_count;
    struct _ax_thread* threads;

    // Main Thread Modules
    uint16_t module_count;
    struct _ax_module* modules;
};

void* _ax_thread_process(void*);

int _ax_is_ok(Artifex ax) {
    // TODO
    return 1;
}

int axInitialize(Artifex* ax, uint8_t threads) {
    if (!ax)
        return 1;

    // Allocate Engine
    *ax = malloc(sizeof(struct _Artifex));

    // Allocate Threads
    (*ax)->thread_count = (threads == 0) ? (sysconf(_SC_NPROCESSORS_ONLN) - 1) : threads;
    (*ax)->threads = ((*ax)->thread_count == 0) ? NULL : malloc(sizeof(struct _ax_thread) * (*ax)->thread_count);

    // Start Threads
    for (uint8_t i = 0; i < (*ax)->thread_count; i++) {
        struct _ax_thread* th = &(*ax)->threads[i];
        th->module_count = 0, th->modules = NULL;
        th->avg_time = 0;
        th->done = 1;  // make thread wait for update (TODO)
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

    // TODO: start threads
    for (uint32_t i = 0; i < ax->thread_count; i++) {
        ax->threads[i].done = 0;
    }

    // TODO wait for threads

    // TODO run main thread stuff

    return 0;
}

void axStartLoop(Artifex ax) {
    while (axUpdate(ax)) {
        // TODO main thread loop
    }
}

id_t axRegister(Artifex ax, const struct axModuleDescriptor* descriptor) {
    if (!_ax_is_ok(ax)) {
        return 1;
    }

    // TODO

    return 0;
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

    while (1) {
        while (th->done);

        // TODO create & update modules, time it

        th->done = 1;
    }

    // TODO destroy modules

    return NULL;
}