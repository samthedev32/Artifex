#pragma once

#include <Math3D/matrix.hpp>
#include <Math3D/vector.hpp>

#include <string>
#include <unordered_map>

#include <GL/glad.h>

namespace Artifex {

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
    void set(std::string n, vec2 vec);

    // Set 3D Vector Uniform
    void set(std::string n, vec3 vec);

    // Set 4 by 4 Matrix Uniform
    void set(std::string n, mat4 mat);
};

} // namespace Artifex