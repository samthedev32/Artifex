#include <Artifex/math/vec/vec3.hpp>

// Constructors & Destructor

vec3::vec3(vec1 xyz) {
  this->x = xyz, this->y = xyz, this->z = xyz;
}
vec3::vec3(vec1 x, vec1 y, vec1 z) {
  this->x = x, this->y = y, this->z = z;
}
vec3::~vec3() {}

// Arithmetic Operators

vec3 vec3::operator+(const vec3 &v) const {
  return {this->x + v.x, this->y + v.y, this->z + v.z};
}
vec3 vec3::operator-(const vec3 &v) const {
  return {this->x - v.x, this->y - v.y, this->z - v.z};
}
vec3 vec3::operator*(const vec3 &v) const {
  return {this->x * v.x, this->y * v.y, this->z * v.z};
}
vec3 vec3::operator/(const vec3 &v) const {
  return {this->x / v.x, this->y / v.y, this->z / v.z};
}

// Assignment Operators

void vec3::operator=(const vec3 &v) {
  this->x = v.x, this->y = v.y, this->z = v.z;
}
void vec3::operator+=(const vec3 &v) { *this = *this + v; }
void vec3::operator-=(const vec3 &v) { *this = *this - v; }
void vec3::operator*=(const vec3 &v) { *this = *this * v; }
void vec3::operator/=(const vec3 &v) { *this = *this / v; }

// Relational Operators

bool vec3::operator==(const vec3 &v) const {
  return this->x == v.x && this->y == v.y && this->z == v.z;
}
bool vec3::operator!=(const vec3 &v) const {
  return this->x != v.x || this->y != v.y || this->z != v.z;
}
bool vec3::operator>(const vec3 &v) const {
  return this->x > v.x && this->y > v.y && this->z > v.z;
}
bool vec3::operator<(const vec3 &v) const {
  return this->x < v.x && this->y < v.y && this->z < v.z;
}
bool vec3::operator>=(const vec3 &v) const {
  return this->x >= v.x && this->y >= v.y && this->z >= v.z;
}
bool vec3::operator<=(const vec3 &v) const {
  return this->x <= v.x && this->y <= v.y && this->z <= v.z;
}

// Other Operators

vec1 vec3::operator[](uint8_t i) const {
  return this->data[i % 3];
}
vec1 &vec3::operator[](uint8_t i) { return this->data[i % 3]; }

void vec3::operator++() { *this += 1; }
void vec3::operator--() { *this -= 1; }

// Functions (Instance Methods)

vec1 vec3::length() const { return sqrtf(dot(*this, *this)); }
vec3 vec3::normalize() const { return *this / length(); }
vec3 vec3::clamp(vec3 min, vec3 max) const {
  return {std::min(std::max(this->x, min.x), max.x),
          std::min(std::max(this->y, min.y), max.y),
          std::min(std::max(this->z, min.z), max.z)};
}

// Functions (Static)

vec1 vec3::distance(vec3 a, vec3 b) {
  return sqrt(dot(a - b, a - b));
}
vec1 vec3::dot(vec3 a, vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
vec3 vec3::lerp(vec3 a, vec3 b, vec1 blend) {
  return {a.x + (b.x - a.x) * blend, a.y + (b.y - a.y) * blend,
          a.y + (b.y - a.y) * blend};
}
vec3 vec3::cross(vec3 a, vec3 b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
          a.x * b.y - a.y * b.x};
}