#pragma once

#include "vec1.hpp"

// 4D Vector
struct vec4 {
  union {
    struct {
      vec1 x, y, z, w;
    };

    struct {
      vec1 r, g, b, a;
    };

    vec1 data[4];
  };

  // Constructors & Destructor

  vec4(vec1 xyzw = 0.0f);
  vec4(vec1 x, vec1 y, vec1 z, vec1 w = 0.0f);

  ~vec4();

  // Arithmetic Operators

  vec4 operator+(const vec4 &v) const;
  vec4 operator-(const vec4 &v) const;
  vec4 operator*(const vec4 &v) const;
  vec4 operator/(const vec4 &v) const;

  // Assignment Operators

  void operator=(const vec4 &v);
  void operator+=(const vec4 &v);
  void operator-=(const vec4 &v);
  void operator*=(const vec4 &v);
  void operator/=(const vec4 &v);

  // Relational Operators

  bool operator==(const vec4 &v) const;
  bool operator!=(const vec4 &v) const;
  bool operator>(const vec4 &v) const;
  bool operator<(const vec4 &v) const;
  bool operator>=(const vec4 &v) const;
  bool operator<=(const vec4 &v) const;

  // Other Operators

  vec1 operator[](uint8_t i) const;
  vec1 &operator[](uint8_t i);

  void operator++();
  void operator--();

  // Functions (Instance Methods)

  vec1 length() const;
  vec4 normalize() const;
  vec4 clamp(vec4 min, vec4 max) const;

  // Functions (Static)

  static vec1 distance(vec4 a, vec4 b);
  static vec1 dot(vec4 a, vec4 b);
  static vec4 lerp(vec4 a, vec4 b, vec1 blend);
};