#pragma once

#include "../vec/vec3.hpp"

// 4 by 4 Matrix
struct mat3 {
  float data[3][3] = {};

  // Constructor & Destructor

  mat3();
  ~mat3();

  // Arithmetic Operators

  mat3 operator*(const mat3 m);
  vec3 operator*(const vec3 v);

  // Assignment Operators

  void operator=(const mat3 m);

  // Relational Operators

  bool operator==(const mat3 &m) const;
  bool operator!=(const mat3 &m) const;

  // Other Operators

  float operator()(uint8_t x, uint8_t y) const;
  float &operator()(uint8_t x, uint8_t y);

  // Functions (Instance Methods)
  mat3 inverse() const;

  // Functions (Static)

  static mat3 identity();

  static mat3 rotationX(float rad);
  static mat3 rotationY(float rad);
  static mat3 rotationZ(float rad);
  static mat3 rotation(vec3 rad);

  static mat3 translation(vec3 v);
  static mat3 scale(vec3 v);

  static mat3 lookAt(vec3 pos, vec3 target, vec3 up);

  static mat3 perspective(float fovrads, float aspect,
                          float fnear, float ffar);

  static mat3 ortho(float left, float right, float bottom,
                    float top, float fnear, float ffar);
};