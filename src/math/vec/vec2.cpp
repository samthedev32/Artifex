#include <Artifex/math/vec/vec2.hpp>

// Constructors & Destructor

vec2::vec2(vec1 xy) { this->x = xy, this->y = xy; }
vec2::vec2(vec1 x, vec1 y) { this->x = x, this->y = y; }
vec2::~vec2() {}

// Arithmetic Operators

vec2 vec2::operator+(const vec2 &v) const {
  return {this->x + v.x, this->y + v.y};
}
vec2 vec2::operator-(const vec2 &v) const {
  return {this->x - v.x, this->y - v.y};
}
vec2 vec2::operator*(const vec2 &v) const {
  return {this->x * v.x, this->y * v.y};
}
vec2 vec2::operator/(const vec2 &v) const {
  return {this->x / v.x, this->y / v.y};
}

// Assignment Operators

void vec2::operator=(const vec2 &v) {
  this->x = v.x, this->y = v.y;
}
void vec2::operator+=(const vec2 &v) { *this = *this + v; }
void vec2::operator-=(const vec2 &v) { *this = *this - v; }
void vec2::operator*=(const vec2 &v) { *this = *this * v; }
void vec2::operator/=(const vec2 &v) { *this = *this / v; }

// Relational Operators

bool vec2::operator==(const vec2 &v) const {
  return this->x == v.x && this->y == v.y;
}
bool vec2::operator!=(const vec2 &v) const {
  return this->x != v.x || this->y != v.y;
}
bool vec2::operator>(const vec2 &v) const {
  return this->x > v.x && this->y > v.y;
}
bool vec2::operator<(const vec2 &v) const {
  return this->x < v.x && this->y < v.y;
}
bool vec2::operator>=(const vec2 &v) const {
  return this->x >= v.x && this->y >= v.y;
}
bool vec2::operator<=(const vec2 &v) const {
  return this->x <= v.x && this->y <= v.y;
}

// Other Operators

vec1 vec2::operator[](uint8_t i) const {
  return this->data[i % 2];
}
vec1 &vec2::operator[](uint8_t i) { return this->data[i % 2]; }

void vec2::operator++() { *this += 1; }
void vec2::operator--() { *this -= 1; }

// Functions (Instance Methods)

vec1 vec2::length() const { return sqrtf(dot(*this, *this)); }
vec2 vec2::normalize() const { return *this / length(); }
vec2 vec2::clamp(vec2 min, vec2 max) const {
  return {std::min(std::max(this->x, min.x), max.x),
          std::min(std::max(this->y, min.y), max.y)};
}

// Functions (Static)

vec1 vec2::distance(vec2 a, vec2 b) {
  return sqrt(dot(a - b, a - b));
}
vec1 vec2::dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
vec2 vec2::lerp(vec2 a, vec2 b, vec1 blend) {
  return {a.x + (b.x - a.x) * blend, a.y + (b.y - a.y) * blend};
}