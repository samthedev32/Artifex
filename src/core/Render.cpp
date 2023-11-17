#include <Artifex/core/Render.hpp>

#include <Artifex/Engine.hpp>

#include <cstring>

// Radian-Degree Conversion Functions
#define rads(d) (d * M_PI / 180.0f)
#define degs(r) (r * 180.0f / M_PI)

namespace Artifex {

void Render::init(Engine *pEngine) {
  if (initialized)
    return;

  engine = pEngine;

  // Load Default Rect
  vec<2> vertices[] = {
      // positions      // texture coords
      {-1.0f, 1.0f},  {0.0f, 1.0f}, // top right
      {-1.0f, -1.0f}, {0.0f, 0.0f}, // bottom right
      {1.0f, -1.0f},  {1.0f, 0.0f}, // bottom left
      {1.0f, 1.0f},   {1.0f, 1.0f}  // top left
  };

  int indices[] = {
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Config OpenGL
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  // Load 2D Shader
  // TODO: default shader
  // engine->load.shader(default_shader_code[0],
  // default_shader_code[1]);
  engine->load.shader("../../../examples/hello_world/shader/2d.glsl");

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
  glClearColor(color->red, color->green, color->blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Render::roundable(const vec<2> &center, const vec<2> &size, int look, const vec<3> &color, uint16_t tex, float amount,
                       float rotation) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, engine->resource.texture[tex].id);

  engine->resource.shader[engine->current.shader].use();

  // Vertex
  engine->resource.shader[engine->current.shader].set("v.center", center);
  engine->resource.shader[engine->current.shader].set("v.size", size);
  engine->resource.shader[engine->current.shader].set("v.ratio", vec<2>(1.0f, engine->ratio()));
  engine->resource.shader[engine->current.shader].set("v.rotation", (float)rads(rotation));

  // Fragment
  engine->resource.shader[engine->current.shader].set("f.look", look);
  engine->resource.shader[engine->current.shader].set("f.corner", amount);
  engine->resource.shader[engine->current.shader].set("f.time", engine->now);

  switch (look) {
  default:
  case 0:
    break;

  case 1:
    engine->resource.shader[engine->current.shader].set("f.color", color);
    break;

  case 2:
    engine->resource.shader[engine->current.shader].set("f.tex", (int)engine->resource.shader[tex].id);
    break;
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
}

void Render::text(const vec<2> &center, float width, const vec<3> &color, float rotation) {
  engine->resource.shader[engine->current.shader].use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, engine->resource.font[0].data.id);

  engine->resource.shader[engine->current.shader].set("v.type", 2);
  engine->resource.shader[engine->current.shader].set("color", color);
  engine->resource.shader[engine->current.shader].set("ratio", engine->ratio());

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
  //                   (c / tfont.height + 1) /
  //                   (float)tfont.height};
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
  //     glBufferData(GL_ARRAY_BUFFER, sizeof(tfont.vertices),
  //     tfont.vertices,
  //                  GL_STATIC_DRAW);

  //     glBindVertexArray(tfont.VAO);
  //     // glDrawArrays(GL_TRIANGLES, 0, 6);
  //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // }
}

} // namespace Artifex