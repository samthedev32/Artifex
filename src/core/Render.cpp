#include <Artifex/core/Render.hpp>

#include <Artifex/Artifex.hpp>

#include <cstring>

#define GL_RECT 0
#define GL_CIRCLE 1

void Render::init(Artifex *artifex) {
    if (initialized)
        return;

    ax = artifex;

    // Load Default Rect
    float vertices[] = {
        // positions      // texture coords
        -1.0f, 1.0f,  0.0f, 1.0f, // top right
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom right
        1.0f,  -1.0f, 1.0f, 0.0f, // bottom left
        1.0f,  1.0f,  1.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Config OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    // Load 2D Shader
    // TODO: default shader
    // ax->load.shader(default_shader_code[0], default_shader_code[1]);
    ax->load.load("../../hello_artifex/shader/2d.glsl");

    // Load Texture
    unsigned char data[3] = {255, 0, 0};
    ax->load.texture(data, 1, 1, 3);

    initialized = true;
}

void Render::deinit() {
    if (!initialized)
        return;

    // Free Textures
    if (ax->texture.size() > 0)
        glDeleteTextures(ax->texture.size(), ax->texture.data());
    ax->texture.clear();

    // Free Shaders
    for (auto s : ax->shader)
        glDeleteShader(s.id);
    ax->shader.clear();

    // Delete Buffers
    GLuint buffers[] = {VAO, VBO};
    glDeleteBuffers(2, buffers);

    initialized = false;
}

void Render::clear(float r, float g, float b) {
    // Clear Screen
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Render::line(vec2 a, vec2 b, vec3 color) {
    // TODO
}

void Render::rect(vec2 center, vec2 size, vec3 color, float rotation) {
    ax->shader[0].use();

    // Vertex
    ax->shader[0].set("center", center);
    ax->shader[0].set("size", vec2(size / 2.0f));
    ax->shader[0].set("ratio", ax->ratio());
    ax->shader[0].set("rotation", rotation);

    // Fragment
    ax->shader[0].set("type", GL_RECT);
    ax->shader[0].set("isTextured", 0);

    ax->shader[0].set("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::rect(vec2 center, vec2 size, uint16_t tex, float rotation) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ax->texture[tex]);

    ax->shader[0].use();

    // Vertex
    ax->shader[0].set("center", center);
    ax->shader[0].set("size", vec2(size / 2.0f));
    ax->shader[0].set("ratio", ax->ratio());
    ax->shader[0].set("rotation", rotation);

    // Fragment
    ax->shader[0].set("type", GL_RECT);
    ax->shader[0].set("isTextured", 1);

    ax->shader[0].set("tex", 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::circle(vec2 center, float radius, vec3 color, float cutradius) {
    ax->shader[0].use();

    // Vertex
    ax->shader[0].set("center", center);
    ax->shader[0].set("size", vec2(radius, radius));
    ax->shader[0].set("ratio", ax->ratio());
    ax->shader[0].set("rotation", 0);

    // Fragment
    ax->shader[0].set("type", GL_CIRCLE);
    ax->shader[0].set("isTextured", 0);

    ax->shader[0].set("color", color);
    ax->shader[0].set("cutradius", cutradius);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::circle(vec2 center, float radius, uint16_t tex, float rotation,
                    float cutradius, vec2 offset) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ax->texture[tex]);

    ax->shader[0].use();

    // Vertex
    ax->shader[0].set("center", center);
    ax->shader[0].set("size", vec2(radius, radius));
    ax->shader[0].set("ratio", ax->ratio());
    ax->shader[0].set("rotation", rotation);

    // Fragment
    ax->shader[0].set("type", GL_CIRCLE);
    ax->shader[0].set("isTextured", 1);

    ax->shader[0].set("tex", 0);
    ax->shader[0].set("cutradius", cutradius);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::text(vec2 center, float width, vec3 color, float rotation) {
    // TODO
}