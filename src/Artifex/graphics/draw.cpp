#include <Artifex/graphics/Renderer.hpp>

#include "GL/glad.h"

#include <ctime>

namespace Artifex {

Shader &Renderer::select(uuid_t shader) {
  if (shaders.count(shader) == 0)
    return shaders[base.shader];

  shaders[shader].use();
  return shaders[shader];
}

void Renderer::draw(uuid_t mesh) {
  if (meshes.count(mesh) == 0)
    return;

  glBindVertexArray(meshes[mesh].VAO);
  glDrawElements(GL_TRIANGLE_FAN, meshes[mesh].size, GL_UNSIGNED_INT, nullptr);
}

void Renderer::draw(const vec<2> &center, const vec<2> &size, float rotation, Look look, const vec<4> &corner,
                    const vec<3> &color, size_t tex) {
  Shader &s = select(base.shader);

  // Vertex
  s.set("v.center", center);
  s.set("v.size", size);
  s.set("v.ratio", vec<2>(1.0f, (float)framebufferSize->width / (float)framebufferSize->height));
  s.set("v.rotation", rotation);

  // Fragment
  s.set("f.look", look);
  s.set("f.corner", corner);

  switch (look) {
  default:
  case DYNAMIC: {
    // TODO: config
    timespec res{};
    clock_gettime(CLOCK_MONOTONIC, &res);
    s.set("f.time",
          static_cast<float>(1000.0f * static_cast<float>(res.tv_sec) + static_cast<float>(res.tv_nsec) / 1e6) / 1000.0f);
  } break;

  case COLOR:
    s.set("f.color", color);
    break;

  case TEXTURE:
    s.set("f.tex", static_cast<int>(tex));
    break;
  }

  draw(base.mesh);
}

// void Renderer::text(const vec<2> &center, float width, const vec<3> &color, std::string text) {
//   Shader &s = select(base.fontShader);

//   s.set("center", center);
//   s.set("width", width);

//   s.set("color", color);
//   s.set("")

//       s.set("f.look", 3);
//   s.set("f.color", color);

//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, engine->resource.font[0].data.id);

//   engine->resource.shader[engine->current.shader].set("v.type", 2);
//   engine->resource.shader[engine->current.shader].set("color", color);
//   engine->resource.shader[engine->current.shader].set("ratio", engine->ratio());

//   // WARNING: Possible Div by 0
//   float step = width / text.size();
//   float left = center.x - step * text.size() / 2.0f;

//   // iterate through all characters
//   for (size_t i = 0; i < text.size(); i++) {
//     char c = text[i] - tfont.start;

//     // Set Position
//     s.set("center", vec<2>(left + i * step, center.y));
//     s.set("size", vec<2>(step, step * height));

//     // WARNING: Possible Divs by 0
//     vec<2> start = {(c % tfont.width) / (float)tfont.width, (c / tfont.height + 1) / (float)tfont.height};
//     vec<2> csize = {1.0f / (float)tfont.width, 1.0f / (float)tfont.height};

//     tfont.vertices[2] = start.x;
//     tfont.vertices[3] = 1 - start.y + csize.y;
//     tfont.vertices[6] = start.x;
//     tfont.vertices[7] = 1 - start.y;
//     tfont.vertices[10] = start.x + csize.x;
//     tfont.vertices[11] = 1 - start.y;

//     tfont.vertices[14] = start.x + csize.x;
//     tfont.vertices[15] = 1 - start.y + csize.y;

//     glBindBuffer(GL_ARRAY_BUFFER, tfont.VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(tfont.vertices), tfont.vertices, GL_STATIC_DRAW);

//     glBindVertexArray(tfont.VAO);
//     // glDrawArrays(GL_TRIANGLES, 0, 6);
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//   }
// }

} // namespace Artifex
