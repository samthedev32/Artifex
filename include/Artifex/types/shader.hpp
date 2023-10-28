#pragma once

#include <Artifex/math/mat/mat.hpp>
#include <Artifex/math/vec/vec.hpp>

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

  // Set 4D Vector Uniform
  void set(std::string n, vec4 vec);

  // Set 3 by 3 Matrix Uniform
  void set(std::string n, mat3 mat);

  // Set 4 by 4 Matrix Uniform
  void set(std::string n, mat4 mat);
};

} // namespace Artifex