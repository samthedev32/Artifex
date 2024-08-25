/**
 * @brief C Rendering Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <Artifex/color.h>
#include <Artifex/vec.h>
#include <Artifex/window.h>

#define AX_RENDERER_DYNAMIC 0
#define AX_RENDERER_COLOR 1
#define AX_RENDERER_TEXTURE 2

// TODO Renderer struct with function pointers to rendering functions
// rather than rendering wrapper, become a rendering standard for libs to be based on
// build UI lib & stuff on it

typedef struct {
    // TODO
} axRendererPtr;

// Artifex Renderer Type
typedef struct axRenderer_* axRenderer;

// Artifex Mesh Type
struct axMesh {
    unsigned int VAO, VBO, EBO;
};

// Check if Renderer is valid
int axRendererIsValid(axRenderer renderer);

// Create Renderer
int axRendererCreate(axRenderer* renderer, axWindow window);

// Destroy Renderer
void axRendererDestroy(axRenderer* renderer);

// Update Renderer
void axRendererUpdate(axRenderer renderer);

// Clear Screen
void axRendererClear(axRenderer renderer, axColor color);

// Load Shader
unsigned int axRendererLoadShader(axRenderer renderer, const char* vertex, const char* fragment, const char* geometry);

// Load Texture
unsigned int axRendererLoadTexture(axRenderer renderer, int width, int height, int channels, const unsigned char* data);

// Load Mesh
struct axMesh axRendererLoadMesh(axRenderer renderer, const axVector* vertices, long v_size, const unsigned int* indices, long i_size);

// Update Texture Data
unsigned int axRendererUpdateTexture(axRenderer renderer, unsigned int texture, int xoffset, int yoffset, int width, int height, int channels, const unsigned char* data);

// Unload Shader
void axRendererUnloadShader(axRenderer renderer, unsigned int shader);

// Unload Texture
void axRendererUnloadTexture(axRenderer renderer, unsigned int texture);

// Unload Mesh
void axRendererUnloadMesh(axRenderer renderer, struct axMesh mesh);

// Draw Info
struct axRendererDrawInfo {
    // Center & Size of Box
    axVector center, size;

    // Rotation of box
    float rotation;

    // Amount of corner-rounding (top-left, top-right, bottom-left, bottom-right)
    float corner[4];

    // Color of rect
    axColor color;

    // Texture of rect
    unsigned int texture;

    // Style of rect
    int style;
};

// Draw
void axRendererDraw(axRenderer renderer, struct axRendererDrawInfo* drawInfo);

// Check for OpenGL Errors
int axRendererCheckErrors();

#ifdef __cplusplus
}
#endif