#include <GL/glad.h>

#include <Artifex/Log.hpp>
#include <Artifex/Renderer.hpp>
#include <cstring>

#include "meshes.h"
#include "shaders.h"

#define TAG "Renderer"

#define rads(deg) (deg * M_PI / 180.0f)
#define degs(rad) (rad * 180.0f / M_PI)

namespace Artifex {
    Renderer::Renderer(Window &window) : m_window(window) {
        m_window.use();

        // Load Default Resources
        m_default.shader = load(RAW::vertex, RAW::fragment);

        auto &uni = m_default.uni;
        auto s = m_default.shader;

        uni.v_center = glGetUniformLocation(s, "v.center");
        uni.v_size = glGetUniformLocation(s, "v.size");
        uni.v_ratio = glGetUniformLocation(s, "v.ratio");
        uni.v_rotation = glGetUniformLocation(s, "v.rotation");
        uni.f_look = glGetUniformLocation(s, "f.look");
        uni.f_corner = glGetUniformLocation(s, "f.corner");
        uni.f_time = glGetUniformLocation(s, "f.time");
        uni.f_color = glGetUniformLocation(s, "f.color");
        uni.f_tex = glGetUniformLocation(s, "f.tex");

        m_default.mesh = load(RAW::vertices, std::size(RAW::vertices), RAW::indices, std::size(RAW::indices));

        // Config OpenGL
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
    }

    Renderer::~Renderer() = default;

    unsigned int Renderer::load(const char *vertex, const char *fragment, const char *geometry) {
        use();

        constexpr size_t minSize = 8;
        if (!vertex || !fragment || strlen(vertex) < minSize || strlen(fragment) < minSize) {
            Log::error(TAG, "load (shader): invalid resources");
            return nil;
        }

        bool isGeo = geometry && strlen(geometry) >= minSize;

        // ReSharper disable once CppDFAUnusedValue
        GLuint geo = 0;

        int success;
        char infoLog[1024];

        // ---- Compile Vertex Shader
        const GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vertex, nullptr);
        glCompileShader(vert);

        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vert, 1024, nullptr, infoLog);
            Log::error(TAG, "failed to compile Vertex Shader:\n%s", infoLog);
            return nil;
        }

        // ---- Compile Fragment Shader
        const GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fragment, nullptr);
        glCompileShader(frag);

        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag, 1024, nullptr, infoLog);
            glDeleteShader(vert);
            printf("Failed to compile Fragment Shader:\n%s", infoLog);
            Log::error(TAG, "failed to compile Fragment Shader:\n%s", infoLog);
            return nil;
        }

        // ---- Compile Geometry Shader (if present)
        if (isGeo) {
            geo = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(geo, 1, &geometry, nullptr);
            glCompileShader(geo);

            glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(geo, 1024, nullptr, infoLog);
                Log::warning(TAG, "failed to compile Geometry Shader:\n%s", infoLog);
                isGeo = false;
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
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            Log::error(TAG, "failed to link shaders:\n%s", infoLog);
            return nil;
        }

        return id;
    }

    unsigned int Renderer::load(int width, int height, int channels, const unsigned char *data) {
        use();

        if (width < 1 || height < 1 || channels < 1 || channels > 4 || !data) {
            Log::error(TAG, "load (texture): invalid resources");
            return nil;
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

        // Create Texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);

        // Generate MipMap & Set Parameters
        // glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        return id;
    }

    mesh_t Renderer::load(const vec<2> *vertices, size_t v_size, const uint32_t *indices, size_t i_size) {
        use();

        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // TODO verify resources

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, v_size * sizeof(*vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size * sizeof(*indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        return {VAO, VBO, EBO};
    }

    void Renderer::unload_shader(unsigned int shader) {
        use();
        glDeleteShader(shader);
    }

    void Renderer::unload(unsigned int texture) {
        use();
        glDeleteTextures(1, &texture);
    }

    void Renderer::unload(mesh_t mesh) {
        use();
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        glDeleteVertexArrays(1, &mesh.VAO);
    }

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

    void Renderer::image(vec<2> center, vec<2> size, unsigned int tex, const Visual &v) const {
        m_window.use();

        glUseProgram(m_default.shader);

        glUniform2f(m_default.uni.v_center, center->x, center->y);
        glUniform2f(m_default.uni.v_size, size->x, size->y);
        glUniform1f(m_default.uni.v_ratio, m_ratio);

        glUniform1i(m_default.uni.f_tex, 0);
        glUniform1f(m_default.uni.v_rotation, rads(v.rotation) + 1.5708);
        glUniform1i(m_default.uni.f_look, 2);
        glUniform4f(m_default.uni.f_corner, v.corner->x, v.corner->y, v.corner->z, v.corner->w);

        glUniform1f(m_default.uni.f_time, Log::now());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        glBindVertexArray(m_default.mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::color(const vec<2> &center, const vec<2> &size, const vec<3> &color, const Visual &v) const {
        m_window.use();

        glUseProgram(m_default.shader);

        glUniform2f(m_default.uni.v_center, center->x, center->y);
        glUniform2f(m_default.uni.v_size, size->x, size->y);
        glUniform1f(m_default.uni.v_ratio, m_ratio);

        glUniform1f(m_default.uni.v_rotation, rads(v.rotation) + 1.5708);
        glUniform1i(m_default.uni.f_look, 0);
        glUniform4f(m_default.uni.f_corner, v.corner->x, v.corner->y, v.corner->z, v.corner->w);

        glUniform1f(m_default.uni.f_time, Log::now());

        glUniform3f(m_default.uni.f_color, color->red, color->green, color->blue);

        glBindVertexArray(m_default.mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::use() {
        m_window.use();
    }

    int Renderer::checkErrors() {
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
