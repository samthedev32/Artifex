#include <Artifex/types/shader.hpp>
#include <iostream>

using namespace Artifex;

Shader::Shader(GLuint id) : id(id) {}

// Use This Shader
void Shader::use() const {
  int program_id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program_id);

  if (program_id != id)
    glUseProgram(this->id);
}

GLint Shader::get(const std::string& n) {
  use();

  if (uniforms.count(n) == 0)
    return uniforms[n] = glGetUniformLocation(this->id, n.c_str());
  return uniforms[n];
}

// 1D Values

void Shader::set(const std::string& n, int value) { glUniform1i(get(n), value); }
void Shader::set(const std::string& n, GLuint value) { glUniform1ui(get(n), value); }
void Shader::set(const std::string& n, float value) { glUniform1f(get(n), value); }
void Shader::set(const std::string& n, double value) { glUniform1d(get(n), value); }

// 2D Values

void Shader::set(const std::string& n, int x, int y) { glUniform2i(get(n), x, y); }
void Shader::set(const std::string& n, GLuint x, GLuint y) { glUniform2ui(get(n), x, y); }
void Shader::set(const std::string& n, float x, float y) { glUniform2f(get(n), x, y); }
void Shader::set(const std::string& n, double x, double y) { glUniform2d(get(n), x, y); }

// 3D Values

void Shader::set(const std::string& n, int x, int y, int z) { glUniform3i(get(n), x, y, z); }
void Shader::set(const std::string& n, GLuint x, GLuint y, GLuint z) { glUniform3ui(get(n), x, y, z); }
void Shader::set(const std::string& n, float x, float y, float z) { glUniform3f(get(n), x, y, z); }
void Shader::set(const std::string& n, double x, double y, double z) { glUniform3d(get(n), x, y, z); }

// 4D Values

void Shader::set(const std::string& n, int x, int y, int z, int w) { glUniform4i(get(n), x, y, z, w); }
void Shader::set(const std::string& n, GLuint x, GLuint y, GLuint z, GLuint w) { glUniform4ui(get(n), x, y, z, w); }
void Shader::set(const std::string& n, float x, float y, float z, float w) { glUniform4f(get(n), x, y, z, w); }
void Shader::set(const std::string& n, double x, double y, double z, double w) { glUniform4d(get(n), x, y, z, w); }

// 1D Vectors

template <> void Shader::set(const std::string& n, vec<1, int> vec) { glUniform1iv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<1, GLuint> vec) { glUniform1uiv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<1, float> vec) { glUniform1fv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<1, double> vec) { glUniform1dv(get(n), 1, vec.data); }

// 2D Vectors

template <> void Shader::set(const std::string& n, vec<2, int> vec) { glUniform2iv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<2, GLuint> vec) { glUniform2uiv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<2, float> vec) { glUniform2fv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<2, double> vec) { glUniform2dv(get(n), 1, vec.data); }

// 3D Vectors

template <> void Shader::set(const std::string& n, vec<3, int> vec) { glUniform3iv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<3, GLuint> vec) { glUniform3uiv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<3, float> vec) { glUniform3fv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<3, double> vec) { glUniform3dv(get(n), 1, vec.data); }

// 4D Vectors

template <> void Shader::set(const std::string& n, vec<4, int> vec) { glUniform4iv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<4, GLuint> vec) { glUniform4uiv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<4, float> vec) { glUniform4fv(get(n), 1, vec.data); }
template <> void Shader::set(const std::string& n, vec<4, double> vec) { glUniform4dv(get(n), 1, vec.data); }

// Matrices

// TODO