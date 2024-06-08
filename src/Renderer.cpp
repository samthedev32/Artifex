#include <Artifex/Renderer.hpp>

#include <Artifex/Log.hpp>

#include "shaders.h"
#include "meshes.h"

#include <GL/glad.h>

#define TAG "Renderer"

#define rads(deg) (deg * M_PI / 180.0f)
#define degs(rad) (rad * 180.0f / M_PI)

namespace Artifex {
    Renderer::Renderer(Window &window, AssetManager &asset) : m_window(window), m_asset(asset) {
        m_window.use();

        // Load Default Resources
        m_default.shader = m_asset.add(RAW::vertex, RAW::fragment, nullptr);

        auto &uni = m_default.uni;
        auto s = m_asset.shader[m_default.shader];

        uni.v_center = glGetUniformLocation(s, "v.center");
        uni.v_size = glGetUniformLocation(s, "v.size");
        uni.v_ratio = glGetUniformLocation(s, "v.ratio");
        uni.v_rotation = glGetUniformLocation(s, "v.rotation");
        uni.f_look = glGetUniformLocation(s, "f.look");
        uni.f_corner = glGetUniformLocation(s, "f.corner");
        uni.f_time = glGetUniformLocation(s, "f.time");
        uni.f_color = glGetUniformLocation(s, "f.color");
        uni.f_tex = glGetUniformLocation(s, "f.tex");

        m_default.mesh = m_asset.add(RAW::vertices, std::size(RAW::vertices), RAW::indices, std::size(RAW::indices));

        // Config OpenGL
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
    }

    Renderer::~Renderer() = default;

    void Renderer::clear(vec<4> color) {
        m_window.use();

        glClearColor(color->red, color->green, color->blue, color->alpha);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::update(const vec<2, int> &size, float ratio) {
        m_size = size;
        m_ratio = ratio;

        m_window.use();

        glViewport(0, 0, m_window.size->width, m_window.size->height);
    }

    void Renderer::image(vec<2> center, vec<2> size, UUID tex, const Visual &v) const {
        m_window.use();

        glUseProgram(m_asset.shader[m_default.shader]);

        glUniform2f(m_default.uni.v_center, center->x, center->y);
        glUniform2f(m_default.uni.v_size, size->x, size->y);
        glUniform1f(m_default.uni.v_ratio, m_ratio);

        glUniform1i(m_default.uni.f_tex, 0);
        glUniform1f(m_default.uni.v_rotation, rads(v.rotation) + 1.5708);
        glUniform1i(m_default.uni.f_look, 2);
        glUniform4f(m_default.uni.f_corner, v.corner->x, v.corner->y, v.corner->z, v.corner->w);

        glUniform1f(m_default.uni.f_time, Log::now());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_asset.texture[tex]);

        glBindVertexArray(m_asset.mesh[m_default.mesh].VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::color(const vec<2> &center, const vec<2> &size, const vec<3> &color, const Visual &v) const {
        m_window.use();

        glUseProgram(m_asset.shader[m_default.shader]);

        glUniform2f(m_default.uni.v_center, center->x, center->y);
        glUniform2f(m_default.uni.v_size, size->x, size->y);
        glUniform1f(m_default.uni.v_ratio, m_ratio);

        glUniform1f(m_default.uni.v_rotation, rads(v.rotation) + 1.5708);
        glUniform1i(m_default.uni.f_look, 0);
        glUniform4f(m_default.uni.f_corner, v.corner->x, v.corner->y, v.corner->z, v.corner->w);

        glUniform1f(m_default.uni.f_time, Log::now());

        glUniform3f(m_default.uni.f_color, color->red, color->green, color->blue);

        glBindVertexArray(m_asset.mesh[m_default.mesh].VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    int Renderer::checkErrors() {
        m_window.use();

        int count = 0;
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            count++;
            std::string error;
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
            Log::warning(TAG, "OpenGL Error: %i (%s)", errorCode, error.c_str());
        }

        return count;
    }
}
