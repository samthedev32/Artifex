#pragma once

#include <EngineToolkit/core/matrix.hpp>
#include <EngineToolkit/core/vector.hpp>

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
    void set(std::string n, EngineToolkit::vec<2> vec);

    // Set 3D Vector Uniform
    void set(std::string n, EngineToolkit::vec<3> vec);

    // Set 4 by 4 Matrix Uniform
    void set(std::string n, EngineToolkit::mat<4> mat);

    // TODO: more vec & mat support
};

} // namespace Artifex