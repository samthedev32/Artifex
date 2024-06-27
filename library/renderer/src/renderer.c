#include <Artifex/clock.h>
#include <Artifex/log.h>
#include <Artifex/renderer.h>
#include <glad/glad.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Artifex/window.h"
#include "res/meshes.h"
#include "res/shaders.h"

#define TAG "axRenderer"

// Radian-Degree Conversion Functions
#define rads(d) (d * M_PI / 180.0f)
#define degs(r) (r * 180.0f / M_PI)

struct axRenderer_ {
    axWindow window;

    // Default Resources (for UI)
    struct {
        unsigned int shader;  // Default 2D Shader
        struct axMesh mesh;   // Default Rectangle Mesh

        // Shader Uniform Locations
        struct {
            int v_center, v_size, v_ratio;
            int v_rotation;
            int f_look, f_corner, f_time, f_color, f_tex;
        } uni;
    } ui;
};

int axRendererIsValid(axRenderer renderer) {
    if (!renderer || !renderer->window)
        return 0;

    return 1;
}

int axRendererCreate(axRenderer* renderer, axWindow window) {
    if (!renderer) {
        ax_error(TAG, "invalid renderer");
        return 1;
    }

    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return 1;
    }

    axRenderer rnd = *renderer = malloc(sizeof(struct axRenderer_));
    if (!rnd) {
        ax_error(TAG, "failed to allocate memory");
        return 1;
    }

    rnd->window = window;

    axWindowMakeCurrent(window);

    // if (!gladLoadGLLoader((GLADloadproc)axWindowGetAddress)) {
    //     free(*renderer);
    //     *renderer = NULL;
    //     return 1;
    // }

    if (!gladLoadGLES2Loader((GLADloadproc)axWindowGetAddress)) {
        free(*renderer);
        *renderer = NULL;
        ax_error(TAG, "failed to initialize opengl");
        return 1;
    }

    // Load Shader
    unsigned int s = rnd->ui.shader = axRendererLoadShader(*renderer, res_vertex, res_fragment, NULL);

    rnd->ui.uni.v_center = glGetUniformLocation(s, "v.center");
    rnd->ui.uni.v_size = glGetUniformLocation(s, "v.size");
    rnd->ui.uni.v_ratio = glGetUniformLocation(s, "v.ratio");
    rnd->ui.uni.v_rotation = glGetUniformLocation(s, "v.rotation");
    rnd->ui.uni.f_look = glGetUniformLocation(s, "f.look");
    rnd->ui.uni.f_corner = glGetUniformLocation(s, "f.corner");
    rnd->ui.uni.f_time = glGetUniformLocation(s, "f.time");
    rnd->ui.uni.f_color = glGetUniformLocation(s, "f.color");
    rnd->ui.uni.f_tex = glGetUniformLocation(s, "f.tex");

    // Load Mesh
    rnd->ui.mesh = axRendererLoadMesh(*renderer, res_vertices, sizeof(res_vertices) / sizeof(*res_vertices), res_indices, sizeof(res_indices) / sizeof(*res_indices));

    // Config OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    ax_verbose(TAG, "initialized renderer");
    return 0;
}

void axRendererDestroy(axRenderer* renderer) {
    if (!renderer || !axRendererIsValid(*renderer)) {
        ax_warning(TAG, "invalid renderer");
        return;
    }

    axWindowMakeCurrent((*renderer)->window);

    axRendererUnloadMesh(*renderer, (*renderer)->ui.mesh);
    axRendererUnloadShader(*renderer, (*renderer)->ui.shader);

    free(*renderer);
    *renderer = NULL;

    ax_verbose(TAG, "destroyed renderer");
}

void axRendererUpdate(axRenderer renderer) {
    if (!axRendererIsValid(renderer))
        return;

    axWindowMakeCurrent(renderer->window);

    // TODO
    int w, h;
    axWindowSize(renderer->window, &w, &h);
    glViewport(0, 0, w, h);
}

void axRendererClear(axRenderer renderer, axColor color) {
    if (!axRendererIsValid(renderer))
        return;

    axWindowMakeCurrent(renderer->window);

    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int axRendererLoadShader(axRenderer renderer, const char* vertex, const char* fragment, const char* geometry) {
    if (!axRendererIsValid(renderer)) {
        ax_error(TAG, "invalid renderer");
        return 0;
    }

    axWindowMakeCurrent(renderer->window);

    const size_t minSize = 8;
    if (!vertex || !fragment || strlen(vertex) < minSize || strlen(fragment) < minSize) {
        ax_error(TAG, "invalid resources");
        return 0;
    }

    int isGeo = geometry && strlen(geometry) >= minSize;

    // ReSharper disable once CppDFAUnusedValue
    GLuint geo = 0;

    int success;
    char infoLog[1024];

    // ---- Compile Vertex Shader
    const GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertex, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 1024, NULL, infoLog);
        ax_error(TAG, "vertex shader error:\n%s", infoLog);
        return 0;
    }

    // ---- Compile Fragment Shader
    const GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragment, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 1024, NULL, infoLog);
        glDeleteShader(vert);
        ax_error(TAG, "fragment shader error:\n%s", infoLog);
        return 0;
    }

    // ---- Compile Geometry Shader (if present)
    if (isGeo) {
        geo = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(geo, 1, &geometry, NULL);
        glCompileShader(geo);

        glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geo, 1024, NULL, infoLog);
            ax_warning(TAG, "geometry shader error:\n%s", infoLog);
            isGeo = 0;
        }
    }

    // ---- Create Shader Program
    GLuint id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    if (isGeo)
        glAttachShader(id, geo);

    glLinkProgram(id);

    // Delete unnecessary shaders
    glDeleteShader(vert);
    glDeleteShader(frag);
    if (isGeo)
        glDeleteShader(geo);

    // Check if linking was successful
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        ax_error(TAG, "failed to link shaders:\n%s", infoLog);
        return 0;
    }

    ax_verbose(TAG, "loaded shader (%i)", id);
    return id;
}

unsigned int axRendererLoadTexture(axRenderer renderer, int width, int height, int channels, const unsigned char* data) {
    if (!axRendererIsValid(renderer)) {
        ax_error(TAG, "invalid renderer");
        return 0;
    }

    axWindowMakeCurrent(renderer->window);

    if (width < 1 || height < 1 || channels < 1 || channels > 4 || !data) {
        ax_error(TAG, "invalid resources");
        return 0;
    }

    GLuint id;
    // Generate Empty Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // TODO verify resources

    // Select Color Mode
    GLint mode;
    switch (channels) {
        case 1:
            mode = GL_RED;
            break;

        case 2:
            mode = GL_RG;
            break;

        default:
        case 3:
            mode = GL_RGB;
            break;

        case 4:
            mode = GL_RGBA;
            break;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);

    // Generate MipMap & Set Parameters
    // glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    ax_verbose(TAG, "loaded texture (%i)", id);
    return id;
}

struct axMesh axRendererLoadMesh(axRenderer renderer, const axVector* vertices, long v_size, const unsigned int* indices, long i_size) {
    if (!axRendererIsValid(renderer)) {
        ax_error(TAG, "invalid renderer");
        return (struct axMesh){};
    }

    axWindowMakeCurrent(renderer->window);

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // TODO verify resources

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size * (long)sizeof(*vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size * (long)sizeof(*indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ax_verbose(TAG, "loaded mesh");
    return (struct axMesh){VAO, VBO, EBO};
}

unsigned int axRendererUpdateTexture(axRenderer renderer, unsigned int texture, int xoffset, int yoffset, int width, int height, int channels, const unsigned char* data) {
    if (!axRendererIsValid(renderer)) {
        ax_error(TAG, "invalid renderer");
        return 0;
    }

    axWindowMakeCurrent(renderer->window);

    // Select Color Mode
    GLint mode;
    switch (channels) {
        case 1:
            mode = GL_RED;
            break;

        case 2:
            mode = GL_RG;
            break;

        default:
        case 3:
            mode = GL_RGB;
            break;

        case 4:
            mode = GL_RGBA;
            break;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, mode, GL_UNSIGNED_BYTE, data);

    return texture;
}

void axRendererUnloadShader(axRenderer renderer, unsigned int shader) {
    if (!axRendererIsValid(renderer)) {
        ax_warning(TAG, "invalid renderer");
        return;
    }

    axWindowMakeCurrent(renderer->window);

    glDeleteShader(shader);

    ax_verbose(TAG, "unloaded shader (%i)", shader);
}

void axRendererUnloadTexture(axRenderer renderer, unsigned int texture) {
    if (!axRendererIsValid(renderer)) {
        ax_warning(TAG, "invalid renderer");
        return;
    }

    axWindowMakeCurrent(renderer->window);

    glDeleteTextures(1, &texture);

    ax_verbose(TAG, "unloaded texture (%i)", texture);
}

void axRendererUnloadMesh(axRenderer renderer, struct axMesh mesh) {
    if (!axRendererIsValid(renderer)) {
        ax_warning(TAG, "invalid renderer");
        return;
    }

    axWindowMakeCurrent(renderer->window);

    glDeleteBuffers(1, &mesh.VBO);
    glDeleteBuffers(1, &mesh.EBO);
    glDeleteVertexArrays(1, &mesh.VAO);

    ax_verbose(TAG, "unloaded mesh");
}

void axRendererDraw(axRenderer renderer, struct axRendererDrawInfo* drawInfo) {
    if (!axRendererIsValid(renderer)) {
        ax_warning(TAG, "invalid renderer");
        return;
    }

    axWindowMakeCurrent(renderer->window);

    glUseProgram(renderer->ui.shader);

    // Set Texture
    if (drawInfo->style == 2) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, drawInfo->texture);
        glUniform1i(renderer->ui.uni.f_tex, 0);
    }

    // Set Position & Size
    glUniform2f(renderer->ui.uni.v_center, drawInfo->center.x, drawInfo->center.y);
    glUniform2f(renderer->ui.uni.v_size, drawInfo->size.x, drawInfo->size.y);

    glUniform1f(renderer->ui.uni.v_ratio, axWindowRatio(renderer->window));

    glUniform1f(renderer->ui.uni.v_rotation, rads(drawInfo->rotation));
    glUniform1i(renderer->ui.uni.f_look, drawInfo->style);
    glUniform4fv(renderer->ui.uni.f_corner, 1, drawInfo->corner);

    glUniform1f(renderer->ui.uni.f_time, axClockNow());

    glUniform3fv(renderer->ui.uni.f_color, 1, (float*)&drawInfo->color);

    glBindVertexArray(renderer->ui.mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

int axRendererCheckErrors() {
    int count = 0;
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        count++;
        const char* error;
        switch (errorCode) {
            default:
                error = "UNKNOWN";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        ax_warning(TAG, "%i", errorCode);
    }

    return count;
}