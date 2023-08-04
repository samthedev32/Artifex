#include <Artifex/core/Render.hpp>

#include <Artifex/Engine.hpp>

#include <cstring>

using namespace Artifex;

#define GL_RECT 0
#define GL_CIRCLE 1

void Render::init(Engine *pEngine) {
    if (initialized)
        return;

    engine = pEngine;

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
    // engine->load.shader(default_shader_code[0], default_shader_code[1]);
    engine->load.load("../../hello_artifex/shader/2d.glsl");

    // Load Texture
    unsigned char data[3] = {255, 0, 0};
    engine->load.texture(data, 1, 1, 3);

    initialized = true;
}

void Render::deinit() {
    if (!initialized)
        return;

    // Free Textures
    if (engine->texture.size() > 0)
        glDeleteTextures(engine->texture.size(), engine->texture.data());
    engine->texture.clear();

    // Free Shaders
    for (auto s : engine->shader)
        glDeleteShader(s.id);
    engine->shader.clear();

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
    engine->shader[0].use();

    // Vertex
    engine->shader[0].set("center", center);
    engine->shader[0].set("size", vec2(size / 2.0f));
    engine->shader[0].set("ratio", engine->ratio());
    engine->shader[0].set("rotation", rotation);

    // Fragment
    engine->shader[0].set("type", GL_RECT);
    engine->shader[0].set("isTextured", 0);

    engine->shader[0].set("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::rect(vec2 center, vec2 size, uint16_t tex, float rotation) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine->texture[tex]);

    engine->shader[0].use();

    // Vertex
    engine->shader[0].set("center", center);
    engine->shader[0].set("size", vec2(size / 2.0f));
    engine->shader[0].set("ratio", engine->ratio());
    engine->shader[0].set("rotation", rotation);

    // Fragment
    engine->shader[0].set("type", GL_RECT);
    engine->shader[0].set("isTextured", 1);

    engine->shader[0].set("tex", 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::circle(vec2 center, float radius, vec3 color, float cutradius) {
    engine->shader[0].use();

    // Vertex
    engine->shader[0].set("center", center);
    engine->shader[0].set("size", vec2(radius, radius));
    engine->shader[0].set("ratio", engine->ratio());
    engine->shader[0].set("rotation", 0);

    // Fragment
    engine->shader[0].set("type", GL_CIRCLE);
    engine->shader[0].set("isTextured", 0);

    engine->shader[0].set("color", color);
    engine->shader[0].set("cutradius", cutradius);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::circle(vec2 center, float radius, uint16_t tex, float rotation,
                    float cutradius, vec2 offset) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine->texture[tex]);

    engine->shader[0].use();

    // Vertex
    engine->shader[0].set("center", center);
    engine->shader[0].set("size", vec2(radius, radius));
    engine->shader[0].set("ratio", engine->ratio());
    engine->shader[0].set("rotation", rotation);

    // Fragment
    engine->shader[0].set("type", GL_CIRCLE);
    engine->shader[0].set("isTextured", 1);

    engine->shader[0].set("tex", 0);
    engine->shader[0].set("cutradius", cutradius);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::text(vec2 center, float width, vec3 color, float rotation) {
    // TODO
}