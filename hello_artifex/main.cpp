#include <Artifex/Artifex.h>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    while (ax.update())
        ax.clear(1.0f, 0.4f, 0.0f);

    return 0;
}