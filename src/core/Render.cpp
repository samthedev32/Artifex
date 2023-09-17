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
        -1.0f, 1.0f,  0.0f, 0.0f, // top right
        -1.0f, -1.0f, 0.0f, 1.0f, // bottom right
        1.0f,  -1.0f, 1.0f, 1.0f, // bottom left
        1.0f,  1.0f,  1.0f, 0.0f  // top left
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
    engine->resource.shader[engine->current.shader].use();

    // Vertex
    engine->resource.shader[engine->current.shader].set("center", center);
    engine->resource.shader[engine->current.shader].set("size", size / 2.0f);
    engine->resource.shader[engine->current.shader].set("ratio",
                                                        engine->ratio());
    engine->resource.shader[engine->current.shader].set("rotation", rotation);

    // Fragment
    engine->resource.shader[engine->current.shader].set("type", GL_RECT);
    engine->resource.shader[engine->current.shader].set("isTextured", 0);

    engine->resource.shader[engine->current.shader].set("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::rect(vec2 center, vec2 size, uint16_t tex, float rotation) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine->resource.texture[tex].id);

    engine->resource.shader[engine->current.shader].use();

    // Vertex
    engine->resource.shader[engine->current.shader].set("center", center);
    engine->resource.shader[engine->current.shader].set("size", size / 2.0f);
    engine->resource.shader[engine->current.shader].set("ratio",
                                                        engine->ratio());
    engine->resource.shader[engine->current.shader].set("rotation", rotation);

    // Fragment
    engine->resource.shader[engine->current.shader].set("type", GL_RECT);
    engine->resource.shader[engine->current.shader].set("isTextured", 1);

    engine->resource.shader[engine->current.shader].set("tex", 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::circle(vec2 center, float radius, vec3 color, float cutradius) {
    engine->resource.shader[engine->current.shader].use();

    // Vertex
    engine->resource.shader[engine->current.shader].set("center", center);
    engine->resource.shader[engine->current.shader].set("size",
                                                        vec2(radius, radius));
    engine->resource.shader[engine->current.shader].set("ratio",
                                                        engine->ratio());
    engine->resource.shader[engine->current.shader].set("rotation", 0);

    // Fragment
    engine->resource.shader[engine->current.shader].set("type", GL_CIRCLE);
    engine->resource.shader[engine->current.shader].set("isTextured", 0);

    engine->resource.shader[engine->current.shader].set("color", color);
    engine->resource.shader[engine->current.shader].set("cutradius", cutradius);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::circle(vec2 center, float radius, uint16_t tex, float rotation,
                    float cutradius, vec2 offset) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine->resource.texture[tex].id);

    engine->resource.shader[engine->current.shader].use();

    // Vertex
    engine->resource.shader[engine->current.shader].set("center", center);
    engine->resource.shader[engine->current.shader].set("size",
                                                        vec2(radius, radius));
    engine->resource.shader[engine->current.shader].set("ratio",
                                                        engine->ratio());
    engine->resource.shader[engine->current.shader].set("rotation", rotation);

    // Fragment
    engine->resource.shader[engine->current.shader].set("type", GL_CIRCLE);
    engine->resource.shader[engine->current.shader].set("isTextured", 1);

    engine->resource.shader[engine->current.shader].set("tex", 0);
    engine->resource.shader[engine->current.shader].set("cutradius", cutradius);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::text(vec2 center, float width, vec3 color, float rotation) {
    engine->resource.shader[engine->current.shader].use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine->resource.font[0].data.id);

    engine->resource.shader[engine->current.shader].set("type", 2);
    engine->resource.shader[engine->current.shader].set("color", color);
    engine->resource.shader[engine->current.shader].set("ratio",
                                                        engine->ratio());

    // WARNING: Possible Div by 0
    // float step = width / text.size();
    // float left = center.x - step * text.size() / 2.0f;

    // // iterate through all characters
    // for (int i = 0; i < (int)text.size(); i++) {
    //     char c = text[i] - tfont.start;

    //     // Set Position
    //     s.set("center", vec2(left + i * step, center.y));
    //     s.set("size", vec2(step, step * height));

    //     // WARNING: Possible Divs by 0
    //     vec2 start = {(c % tfont.width) / (float)tfont.width,
    //                   (c / tfont.height + 1) / (float)tfont.height};
    //     vec2 csize = {1.0f / (float)tfont.width, 1.0f / (float)tfont.height};

    //     tfont.vertices[2] = start.x;
    //     tfont.vertices[3] = 1 - start.y + csize.y;
    //     tfont.vertices[6] = start.x;
    //     tfont.vertices[7] = 1 - start.y;
    //     tfont.vertices[10] = start.x + csize.x;
    //     tfont.vertices[11] = 1 - start.y;

    //     tfont.vertices[14] = start.x + csize.x;
    //     tfont.vertices[15] = 1 - start.y + csize.y;

    //     glBindBuffer(GL_ARRAY_BUFFER, tfont.VBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(tfont.vertices), tfont.vertices,
    //                  GL_STATIC_DRAW);

    //     glBindVertexArray(tfont.VAO);
    //     // glDrawArrays(GL_TRIANGLES, 0, 6);
    //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // }
}