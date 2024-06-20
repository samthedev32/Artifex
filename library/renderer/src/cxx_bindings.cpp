#include <Artifex/renderer.h>

#include <Artifex/renderer.hpp>
#include <Artifex/window.hpp>
#include <cstring>
#include <stdexcept>

namespace Artifex {

struct Renderer::Mesh {
    unsigned int VAO, VBO, EBO;
};

Renderer::Renderer(Window& window) {
    if (axRendererCreate((axRenderer*)&m_renderer, (axWindow)window.m_window))
        throw std::runtime_error("failed to create renderer");
}

Renderer::~Renderer() {
    axRendererDestroy((axRenderer*)&m_renderer);
}

void Renderer::update() {
    axRendererUpdate((axRenderer)m_renderer);
}

void Renderer::clear(const vec<4>& color) {
    axRendererClear((axRenderer)m_renderer, color);
}

unsigned int Renderer::loadShader(const char* vertex, const char* fragment, const char* geometry) {
    return axRendererLoadShader((axRenderer)m_renderer, vertex, fragment, geometry);
}

unsigned int Renderer::loadTexture(int width, int height, int channels, const unsigned char* data) {
    return axRendererLoadTexture((axRenderer)m_renderer, width, height, channels, data);
}

Renderer::Mesh Renderer::loadMesh(const std::vector<vec<2>>& vertices, const std::vector<unsigned int>& indices) {
    axMesh out = axRendererLoadMesh((axRenderer)m_renderer, (axVector*)vertices.data(), (long)vertices.size(), (unsigned int*)indices.data(), (long)indices.size());
    return {out.VAO, out.VBO, out.EBO};
}

void Renderer::unloadShader(unsigned int shader) {
    axRendererUnloadShader((axRenderer)m_renderer, shader);
}

void Renderer::unloadTexture(unsigned int texture) {
    axRendererUnloadTexture((axRenderer)m_renderer, texture);
}

void Renderer::unloadMesh(Renderer::Mesh mesh) {
    axRendererUnloadMesh((axRenderer)m_renderer, {mesh.VAO, mesh.VBO, mesh.EBO});
}

void Renderer::draw(const Renderer::DrawInfo& drawInfo) {
    axRendererDrawInfo info{};
    info.center = drawInfo.center;
    info.size = drawInfo.size;
    info.rotation = drawInfo.rotation;
    memcpy(info.corner, drawInfo.corner.data, 4 * sizeof(float));
    info.color = drawInfo.color;
    info.texture = drawInfo.texture;
    info.style = drawInfo.style;

    axRendererDraw((axRenderer)m_renderer, &info);
}

}  // namespace Artifex