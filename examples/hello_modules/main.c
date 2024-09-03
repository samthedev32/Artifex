#include <Artifex/engine.h>
#include <stdio.h>

// global TODO
// maybe scheduler to separate file
// rename it to "somewhat scheduler" (SWS for short) xd
// module callbacks

void upd() {
    printf("asd\n");
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

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    info.user = (void*)2;
    axRegister(ax, &info);

    axStartLoop(ax);

    return 0;
}