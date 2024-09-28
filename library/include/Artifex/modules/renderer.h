/**
 * @brief Basic 2D Renderer for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <Artifex/engine.h>

struct axRendererDescriptor {
    // Window Config
    const char* windowTitle;
    uint32_t width, height;
    uint8_t vsync;          // 0 == none
    uint32_t refresh_rate;  // 0 == unlimited
};

// Initialize Renderer
struct axModuleDescriptor* axInitRenderer(struct axRendererDescriptor* descriptor);

#ifdef __cplusplus
}
#endif