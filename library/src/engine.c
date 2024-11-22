#include <Artifex/engine.h>
#include <Artifex/error.h>
#include <Artifex/fundef.h>

#include <Artifex/ecs/typeless.h>

//
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Module Backend
struct _ax_module {
    int enabled;
    float last_upd;

    // Global User Data
    void* user;

    // Callback Function Pointers
    _ax_fn_create_t onCreate;
    _ax_fn_destroy_t onDestroy;
    _ax_fn_update_t onUpdate;
};

// Engine Backend
struct _Artifex {
    // Modules
    uint64_t module_count;
    struct _ax_module* modules;
 
    Typeless ecs;
};

uint64_t _ax_now() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return t.tv_sec * 1000000 + t.tv_nsec / 1000;
}

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

    (*ax)->module_count = 0;
    (*ax)->modules = NULL;

    tlInitialize((*ax)->ecs);

    // TODO

    return AX_OK;
}

void axDestroy(Artifex* ax) {
    if (!ax || !_ax_is_ok(*ax)) {
        return;
    }

    // TODO
    tlDestroy((*ax)->ecs);

    return;
}

int axUpdate(Artifex ax) {
    if (!_ax_is_ok(ax)) {
        return AX_INVALID_INPUT;
    }

    // Update Modules
    uint64_t time = _ax_now();
    struct axModuleInfo info;
    for (uint64_t i = 0; i < ax->module_count; i++) {
        struct _ax_module* m = &ax->modules[i];
        if (m->enabled) {
            info.id = i;
            info.engine = ax;

            // Initialize Module if first run
            if (m->last_upd == 0) {
                m->onCreate ? m->onCreate(&info, m->user) : NULL;
                m->last_upd = time;
                continue;
            }

            info.delta = time - m->last_upd;
            m->last_upd = time;
            m->onUpdate ? m->onUpdate(&info, m->user) : NULL;
        }
    }

    return AX_OK;
}

int axStartLoop(Artifex ax) {
    if (!_ax_is_ok(ax))
        return AX_INVALID_INPUT;

    while (!axUpdate(ax));

    return AX_OK;
}

uint64_t axRegister(Artifex ax, const struct axModuleDescriptor* descriptor) {
    if (!_ax_is_ok(ax))
        return 0;

    struct _ax_module* newp = realloc(ax->modules, sizeof(*ax->modules) * (ax->module_count + 1));
    if (!newp) {
        return 0;
    }

    uint64_t id = ax->module_count;
    newp[id].enabled = 1;
    newp[id].last_upd = 0;

    newp[id].onCreate = descriptor->onCreate;
    newp[id].onDestroy = descriptor->onDestroy;
    newp[id].onUpdate = descriptor->onUpdate;
    newp[id].user = descriptor->user;

    // TODO impl dependencies

    ax->module_count++;
    ax->modules = newp;
    return ax->module_count;
}

int axEnable(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return AX_INVALID_INPUT;
    }

    // TODO

    return AX_OK;
}

int axDisable(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return AX_INVALID_INPUT;
    }

    // TODO

    return AX_OK;
}
