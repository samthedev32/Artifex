#include <Artifex/math/vec/vec4.hpp>

// Constructors & Destructor

vec4::vec4(vec1 xyzw) {
  this->x = xyzw, this->y = xyzw, this->z = xyzw,
  this->w = xyzw;
}
vec4::vec4(vec1 x, vec1 y, vec1 z, vec1 w) {
  this->x = x, this->y = y, this->z = z, this->w = w;
}
vec4::~vec4() {}

// Arithmetic Operators

vec4 vec4::operator+(const vec4 &v) const {
  return {this->x + v.x, this->y + v.y, this->z + v.z,
          this->w + v.w};
}
vec4 vec4::operator-(const vec4 &v) const {
  return {this->x - v.x, this->y - v.y, this->z - v.z,
          this->w - v.w};
}
vec4 vec4::operator*(const vec4 &v) const {
  return {this->x * v.x, this->y * v.y, this->z * v.z,
          this->w * v.w};
}
vec4 vec4::operator/(const vec4 &v) const {
  return {this->x / v.x, this->y / v.y, this->z / v.z,
          this->w / v.w};
}

// Assignment Operators

void vec4::operator=(const vec4 &v) {
  this->x = v.x, this->y = v.y, this->z = v.z, this->w = v.w;
}
void vec4::operator+=(const vec4 &v) { *this = *this + v; }
void vec4::operator-=(const vec4 &v) { *this = *this - v; }
void vec4::operator*=(const vec4 &v) { *this = *this * v; }
void vec4::operator/=(const vec4 &v) { *this = *this / v; }

// Relational Operators

bool vec4::operator==(const vec4 &v) const {
  return this->x == v.x && this->y == v.y && this->z == v.z
         && this->w == v.w;
}
bool vec4::operator!=(const vec4 &v) const {
  return this->x != v.x || this->y != v.y || this->z != v.z
         || this->w != v.w;
}
bool vec4::operator>(const vec4 &v) const {
  return this->x > v.x && this->y > v.y && this->z > v.z
         && this->w > v.w;
}
bool vec4::operator<(const vec4 &v) const {
  return this->x < v.x && this->y < v.y && this->z < v.z
         && this->w < v.w;
}
bool vec4::operator>=(const vec4 &v) const {
  return this->x >= v.x && this->y >= v.y && this->z >= v.z
         && this->w >= v.w;
}
bool vec4::operator<=(const vec4 &v) const {
  return this->x <= v.x && this->y <= v.y && this->z <= v.z
         && this->w <= v.w;
}

// Other Operators

vec1 vec4::operator[](uint8_t i) const {
  return this->data[i % 4];
}
vec1 &vec4::operator[](uint8_t i) { return this->data[i % 4]; }

void vec4::operator++() { *this += 1; }
void vec4::operator--() { *this -= 1; }

// Functions (Instance Methods)

vec1 vec4::length() const { return sqrtf(dot(*this, *this)); }
vec4 vec4::normalize() const { return *this / length(); }
vec4 vec4::clamp(vec4 min, vec4 max) const {
  return {std::min(std::max(this->x, min.x), max.x),
          std::min(std::max(this->y, min.y), max.y),
          std::min(std::max(this->z, min.z), max.z),
          std::min(std::max(this->w, min.w), max.w)};
}

// Functions (Static)

vec1 vec4::distance(vec4 a, vec4 b) {
  return sqrt(dot(a - b, a - b));
}
vec1 vec4::dot(vec4 a, vec4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
vec4 vec4::lerp(vec4 a, vec4 b, vec1 blend) {
  return {a.x + (b.x - a.x) * blend, a.y + (b.y - a.y) * blend,
          a.z + (b.z - a.z) * blend, a.w + (b.w - a.w) * blend};
}