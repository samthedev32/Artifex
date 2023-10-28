#pragma once

#include "vec1.hpp"

// 2D Vector
struct vec2 {
  union {
    struct {
      vec1 x, y;
    };

    struct {
      vec1 u, v;
    };

    struct {
      vec1 width, height;
    };

    vec1 data[2];
  };

  // Constructors & Destructor

  vec2(vec1 xy = 0.0f);
  vec2(vec1 x, vec1 y);

  ~vec2();

  // Arithmetic Operators

  vec2 operator+(const vec2 &v) const;
  vec2 operator-(const vec2 &v) const;
  vec2 operator*(const vec2 &v) const;
  vec2 operator/(const vec2 &v) const;

  // Assignment Operators

  void operator=(const vec2 &v);
  void operator+=(const vec2 &v);
  void operator-=(const vec2 &v);
  void operator*=(const vec2 &v);
  void operator/=(const vec2 &v);

  // Relational Operators

  bool operator==(const vec2 &v) const;
  bool operator!=(const vec2 &v) const;
  bool operator>(const vec2 &v) const;
  bool operator<(const vec2 &v) const;
  bool operator>=(const vec2 &v) const;
  bool operator<=(const vec2 &v) const;

  // Other Operators

  vec1 operator[](uint8_t i) const;
  vec1 &operator[](uint8_t i);

  void operator++();
  void operator--();

  // Functions (Instance Methods)

  vec1 length() const;
  vec2 normalize() const;
  vec2 clamp(vec2 min, vec2 max) const;

  // Functions (Static)

  static vec1 distance(vec2 a, vec2 b);
  static vec1 dot(vec2 a, vec2 b);
  static vec2 lerp(vec2 a, vec2 b, vec1 blend);
};