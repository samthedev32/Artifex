#include <Artifex/engine.h>
#include <Artifex/error.h>
#include <Artifex/fundef.h>

//
#include <malloc.h>
#include <stdint.h>

// Module Backend
struct _ax_module {
    int enabled;

    // User Data
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

    // TODO ECS
};

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

    // TODO

    return AX_OK;
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
        return AX_INVALID_INPUT;
    }

    // Update Modules
    struct axModuleInfo info;
    for (uint64_t i = 0; i < ax->module_count; i++) {
        struct _ax_module* m = &ax->modules[i];
        if (m->enabled) {
            info.id = i;
            info.engine = ax;
            info.delta = 0;  // TODO

            m->onUpdate(&info, m->user);
        }
    }

    return AX_OK;
}

int axStartLoop(Artifex ax) {
    if (!_ax_is_ok(ax))
        return AX_INVALID_INPUT;

    while (!axUpdate(ax)) {
        // for (uint32_t i = 0; i < ax->thread_count; ax++)
        // ax->threads[i].done = 0;
        // TODO main thread loop
    }

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

int axEnableTree(Artifex ax, id_t moduleID) {
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

int axDisableTree(Artifex ax, id_t moduleID) {
    if (!_ax_is_ok(ax)) {
        return AX_INVALID_INPUT;
    }

    // TODO

    return AX_OK;
}