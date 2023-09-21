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
    vec<2> vertices[] = {
        // positions      // texture coords
        {-1.0f, 1.0f},  {0.0f, 0.0f}, // top right
        {-1.0f, -1.0f}, {0.0f, 1.0f}, // bottom right
        {1.0f, -1.0f},  {1.0f, 1.0f}, // bottom left
        {1.0f, 1.0f},   {1.0f, 0.0f}  // top left
    };

    vec<3, int> indices[] = {
        {0, 1, 3}, // first triangle
        {1, 2, 3}  // second triangle
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

void Render::clear(vec<3> color) {
    // Clear Screen
    glClearColor(color->r, color->g, color->b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Render::rounded(vec<2> center, vec<2> size, uint16_t tex, float amount,
                     float rotation) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine->resource.texture[tex].id);

    engine->resource.shader[engine->current.shader].use();

    // Vertex
    engine->resource.shader[engine->current.shader].set("center", center);
    engine->resource.shader[engine->current.shader].set("size", size);
    engine->resource.shader[engine->current.shader].set(
        "ratio", vec<2>(1.0f, engine->ratio()));
    engine->resource.shader[engine->current.shader].set("rotation",
                                                        (float)rads(rotation));

    // Fragment
    engine->resource.shader[engine->current.shader].set("isTextured", 1);

    engine->resource.shader[engine->current.shader].set("tex", 0);

    engine->resource.shader[engine->current.shader].set("corner", amount);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
}

void Render::text(vec<2> center, float width, vec<3> color, float rotation) {
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
    //     s.set("center", vec<2>(left + i * step, center.y));
    //     s.set("size", vec<2>(step, step * height));

    //     // WARNING: Possible Divs by 0
    //     vec<2> start = {(c % tfont.width) / (float)tfont.width,
    //                   (c / tfont.height + 1) / (float)tfont.height};
    //     vec<2> csize = {1.0f / (float)tfont.width, 1.0f /
    //     (float)tfont.height};

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