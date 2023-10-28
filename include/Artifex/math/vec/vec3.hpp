#pragma once

#include "vec1.hpp"

// 3D Vector
struct vec3 {
  union {
    struct {
      vec1 x, y, z;
    };

    struct {
      vec1 r, g, b;
    };

    struct {
      vec1 u, v, w;
    };

    struct {
      vec1 width, height, depth;
    };

    vec1 data[3];
  };

  // Constructors & Destructor

  vec3(vec1 xyz = 0.0f);
  vec3(vec1 x, vec1 y, vec1 z);

  ~vec3();

  // Arithmetic Operators

  vec3 operator+(const vec3 &v) const;
  vec3 operator-(const vec3 &v) const;
  vec3 operator*(const vec3 &v) const;
  vec3 operator/(const vec3 &v) const;

  // Assignment Operators

  void operator=(const vec3 &v);
  void operator+=(const vec3 &v);
  void operator-=(const vec3 &v);
  void operator*=(const vec3 &v);
  void operator/=(const vec3 &v);

  // Relational Operators

  bool operator==(const vec3 &v) const;
  bool operator!=(const vec3 &v) const;
  bool operator>(const vec3 &v) const;
  bool operator<(const vec3 &v) const;
  bool operator>=(const vec3 &v) const;
  bool operator<=(const vec3 &v) const;

  // Other Operators

  vec1 operator[](uint8_t i) const;
  vec1 &operator[](uint8_t i);

  void operator++();
  void operator--();

  // Functions (Instance Methods)

  vec1 length() const;
  vec3 normalize() const;
  vec3 clamp(vec3 min, vec3 max) const;

  // Functions (Static)

  static vec1 distance(vec3 a, vec3 b);
  static vec1 dot(vec3 a, vec3 b);
  static vec3 lerp(vec3 a, vec3 b, vec1 blend);
  static vec3 cross(vec3 a, vec3 b);
};