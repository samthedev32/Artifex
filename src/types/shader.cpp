#include <Artifex/types/shader.hpp>

Shader::Shader(GLuint id) : id(id) {}

// Use This Shader
void Shader::use() {
    int program_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program_id);

    if (program_id != id)
        glUseProgram(this->id);
}

// Set Boolean Uniform
void Shader::set(std::string n, bool value) {
    use();

    if (uniforms[n] == 0)
        uniforms[n] = glGetUniformLocation(this->id, n.c_str()) + 1;
    glUniform1i(uniforms[n] - 1, (int)value);
}

// Set Integer Uniform
void Shader::set(std::string n, int value) {
    use();

    if (uniforms[n] == 0)
        uniforms[n] = glGetUniformLocation(this->id, n.c_str()) + 1;
    glUniform1i(uniforms[n] - 1, value);
}

// Set Float Uniform
void Shader::set(std::string n, float value) {
    use();

    if (uniforms[n] == 0)
        uniforms[n] = glGetUniformLocation(this->id, n.c_str()) + 1;
    glUniform1f(uniforms[n] - 1, value);
}

// Set 2D Vector Uniform
void Shader::set(std::string n, vec2 vec) {
    use();

    if (uniforms[n] == 0)
        uniforms[n] = glGetUniformLocation(this->id, n.c_str()) + 1;
    glUniform2f(uniforms[n] - 1, vec.x, vec.y);
}

// Set 3D Vector Uniform
void Shader::set(std::string n, vec3 vec) {
    use();

    if (uniforms[n] == 0)
        uniforms[n] = glGetUniformLocation(this->id, n.c_str()) + 1;
    glUniform3f(uniforms[n] - 1, vec.x, vec.y, vec.z);
}

// Set 4 by 4 Matrix Uniform
void Shader::set(std::string n, mat4 mat) {
    use();

    if (uniforms[n] == 0)
        uniforms[n] = glGetUniformLocation(this->id, n.c_str()) + 1;
    glUniformMatrix4fv(uniforms[n] - 1, 1, GL_FALSE, &mat.m[0][0]);
}