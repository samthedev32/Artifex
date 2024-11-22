#include <Artifex/engine.h>
#include <stdio.h>

void upd(const struct axModuleInfo* info, void* user) {
    printf("module %lu: asd %i\n", info->id, user);
}

int main() {
    Artifex ax;
    axInitialize(&ax, 0);

    struct axModuleDescriptor info = {};
    info.onUpdate = upd;

    info.user = (void*)1;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    axRegister(ax, &info);
    info.user = (void*)2;

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    axStartLoop(ax);

    return 0;
}