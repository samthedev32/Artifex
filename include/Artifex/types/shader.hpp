#pragma once

#include <EngineToolkit/EngineToolkit.hpp>

#include <string>
#include <unordered_map>

#include <GL/glad.h>

namespace Artifex {

using namespace EngineToolkit;

// OpenGL Shader Library for Artifex
struct Shader {
  private:
    // Uniform Locations
    std::unordered_map<std::string, GLuint> uniforms;

  public:
    Shader(GLuint id = 0);

    // The Shader Program ID
    GLuint id;

    // Use This Shader
    void use();

    // Set Boolean Uniform
    void set(std::string n, bool value);

    // Set Integer Uniform
    void set(std::string n, int value);

    // Set Float Uniform
    void set(std::string n, float value);

    // Set 2D Vector Uniform
    void set(std::string n, vec<2> vec);

    // Set 3D Vector Uniform
    void set(std::string n, vec<3> vec);

    // Set 4 by 4 Matrix Uniform
    void set(std::string n, mat<4> mat);

    // Set Vector
    template <uint8_t D, typename T> void set(std::string n, vec<D, T> vec);

    // Set Matrix
    template <uint8_t R, uint8_t C, typename T>
    void set(std::string n, mat<R, C, T> mat);
};

// Set Vector
template <uint8_t D, typename T>
void Shader::set(std::string n, vec<D, T> vec) {
    // TODO
}

// Set Matrix
template <uint8_t R, uint8_t C, typename T>
void Shader::set(std::string n, mat<R, C, T> mat) {
    // TODO
}

} // namespace Artifex