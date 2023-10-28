#pragma once

#include "../vec/vec3.hpp"

// 4 by 4 Matrix
struct mat4 {
  float data[4][4] = {};

  // Constructor & Destructor

  mat4();
  ~mat4();

  // Arithmetic Operators

  mat4 operator*(const mat4 m);
  vec3 operator*(const vec3 v);

  // Assignment Operators

  void operator=(const mat4 m);

  // Relational Operators

  bool operator==(const mat4 &m) const;
  bool operator!=(const mat4 &m) const;

  // Other Operators

  float operator()(uint8_t x, uint8_t y) const;
  float &operator()(uint8_t x, uint8_t y);

  // Functions (Instance Methods)
  mat4 inverse() const;

  // Functions (Static)

  static mat4 identity();

  static mat4 rotationX(float rad);
  static mat4 rotationY(float rad);
  static mat4 rotationZ(float rad);
  static mat4 rotation(vec3 rad);

  static mat4 translation(vec3 v);
  static mat4 scale(vec3 v);

  static mat4 lookAt(vec3 pos, vec3 target, vec3 up);

  static mat4 perspective(float fovrads, float aspect,
                          float fnear, float ffar);

  static mat4 ortho(float left, float right, float bottom,
                    float top, float fnear, float ffar);
};