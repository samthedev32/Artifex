//
// Created by SamTheDev on 11/12/2023.
//

#include <Artifex/core/renderer.hpp>

namespace Artifex {

void Renderer::draw::color(const vec<3> &color) {
  // TODO optimize using draw.mesh
  renderer.shaders[0].use();

  // Vertex
  renderer.shaders[0].set("v.center", vec<2>{});
  renderer.shaders[0].set("v.size", vec<2>{0.5f});
  renderer.shaders[0].set("v.ratio", vec<2>(1.0f, renderer.ratio));
  renderer.shaders[0].set("v.rotation", (float)0.0f);

  // Fragment
  renderer.shaders[0].set("f.look", 1);
  renderer.shaders[0].set("f.corner", 0.0f); // TODO determine by shape

  renderer.shaders[0].set("f.color", color);

  glBindVertexArray(renderer.meshes[0].VAO);
  glDrawElements(GL_TRIANGLE_FAN, renderer.meshes[0].size, GL_UNSIGNED_INT, nullptr);
}

// void Renderer::roundable(const vec<2> &center, const vec<2> &size, int look, const vec<3> &color, uint16_t tex, float amount,
//                          float rotation) {
//   shaders[0].use();
//
//   // Vertex
//   shaders[0].set("v.center", center);
//   shaders[0].set("v.size", size);
//   shaders[0].set("v.ratio", vec<2>(1.0f, (float)Window::size->width / (float)Window::size->height));
//   shaders[0].set("v.rotation", (float)rads(rotation));
//
//   // Fragment
//   shaders[0].set("f.look", look);
//   shaders[0].set("f.corner", amount);
//
//   switch (look) {
//   default:
//   case 0: {
//     struct timespec res {};
//     clock_gettime(CLOCK_MONOTONIC, &res);
//     const float now
//         = (1000.0f * static_cast<float>(res.tv_sec) + static_cast<float>(res.tv_nsec) / static_cast<float>(1e6)) / 1000.0f;
//     shaders[0].set("f.time", now); // TODO
//   } break;
//
//   case 1:
//     shaders[0].set("f.color", color);
//     break;
//
//   case 2:
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, textures[tex]);
//     shaders[0].set("f.tex", textures[tex]);
//     break;
//   }
//
//   glBindVertexArray(meshes[0].VAO);
//   glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
// }

// void Renderer::text(const vec<2> &center, float width, const vec<3> &color, float rotation) {
//   engine->resource.shader[engine->current.shader].use();
//
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, engine->resource.font[0].data.id);
//
//   engine->resource.shader[engine->current.shader].set("v.type", 2);
//   engine->resource.shader[engine->current.shader].set("color", color);
//   engine->resource.shader[engine->current.shader].set("ratio", engine->ratio());
//
//   // WARNING: Possible Div by 0
//   // float step = width / text.size();
//   // float left = center.x - step * text.size() / 2.0f;
//
//   // // iterate through all characters
//   // for (int i = 0; i < (int)text.size(); i++) {
//   //     char c = text[i] - tfont.start;
//
//   //     // Set Position
//   //     s.set("center", vec<2>(left + i * step, center.y));
//   //     s.set("size", vec<2>(step, step * height));
//
//   //     // WARNING: Possible Divs by 0
//   //     vec<2> start = {(c % tfont.width) / (float)tfont.width,
//   //                   (c / tfont.height + 1) /
//   //                   (float)tfont.height};
//   //     vec<2> csize = {1.0f / (float)tfont.width, 1.0f /
//   //     (float)tfont.height};
//
//   //     tfont.vertices[2] = start.x;
//   //     tfont.vertices[3] = 1 - start.y + csize.y;
//   //     tfont.vertices[6] = start.x;
//   //     tfont.vertices[7] = 1 - start.y;
//   //     tfont.vertices[10] = start.x + csize.x;
//   //     tfont.vertices[11] = 1 - start.y;
//
//   //     tfont.vertices[14] = start.x + csize.x;
//   //     tfont.vertices[15] = 1 - start.y + csize.y;
//
//   //     glBindBuffer(GL_ARRAY_BUFFER, tfont.VBO);
//   //     glBufferData(GL_ARRAY_BUFFER, sizeof(tfont.vertices),
//   //     tfont.vertices,
//   //                  GL_STATIC_DRAW);
//
//   //     glBindVertexArray(tfont.VAO);
//   //     // glDrawArrays(GL_TRIANGLES, 0, 6);
//   //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//   // }
// }

} // namespace Artifex
