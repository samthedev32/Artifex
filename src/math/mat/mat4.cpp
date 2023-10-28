#include <Artifex/math/mat/mat4.hpp>

// Constructor & Destructor

mat4::mat4() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      this->data[i][j] = 0.0f;
}
mat4::~mat4() {}

// Arithmetic Operators

mat4 mat4::operator*(const mat4 m) {
  mat4 matrix;
  for (int c = 0; c < 4; c++)
    for (int r = 0; r < 4; r++)
      matrix.data[r][c] = this->data[r][0] * m.data[0][c]
                          + this->data[r][1] * m.data[1][c]
                          + this->data[r][2] * m.data[2][c]
                          + this->data[r][3] * m.data[3][c];
  return matrix;
}

vec3 mat4::operator*(const vec3 v) {
  return vec3(
      v.x * this->data[0][0] + v.y * this->data[1][0]
          + v.z * this->data[2][0] + 1 * this->data[3][0],
      v.x * this->data[0][1] + v.y * this->data[1][1]
          + v.z * this->data[2][1] + 1 * this->data[3][1],
      v.x * this->data[0][2] + v.y * this->data[1][2]
          + v.z * this->data[2][2] + 1 * this->data[3][2]);
}

// Assignment Operators

void mat4::operator=(const mat4 m) {
  for (int c = 0; c < 4; c++)
    for (int r = 0; r < 4; r++)
      this->data[r][c] = m.data[r][c];
}

// Relational Operators

bool mat4::operator==(const mat4 &m) const {
  for (int c = 0; c < 4; c++)
    for (int r = 0; r < 4; r++)
      if (this->data[r][c] != m.data[r][c])
        return false;
  return true;
}

bool mat4::operator!=(const mat4 &m) const {
  for (int c = 0; c < 4; c++)
    for (int r = 0; r < 4; r++)
      if (this->data[r][c] == m.data[r][c])
        return false;
  return true;
}

// Other Operators

float mat4::operator()(uint8_t x, uint8_t y) const {
  return this->data[x % 4][y % 4];
}

float &mat4::operator()(uint8_t x, uint8_t y) {
  return this->data[x % 4][y % 4];
}

// Functions (Instance Methods)

mat4 mat4::inverse() const {
  mat4 matrix;
  matrix.data[0][0] = this->data[0][0];
  matrix.data[0][1] = this->data[1][0];
  matrix.data[0][2] = this->data[2][0];
  matrix.data[0][3] = 0.0f;
  matrix.data[1][0] = this->data[0][1];
  matrix.data[1][1] = this->data[1][1];
  matrix.data[1][2] = this->data[2][1];
  matrix.data[1][3] = 0.0f;
  matrix.data[2][0] = this->data[0][2];
  matrix.data[2][1] = this->data[1][2];
  matrix.data[2][2] = this->data[2][2];
  matrix.data[2][3] = 0.0f;
  matrix.data[3][0] = -(this->data[3][0] * matrix.data[0][0]
                        + this->data[3][1] * matrix.data[1][0]
                        + this->data[3][2] * matrix.data[2][0]);
  matrix.data[3][1] = -(this->data[3][0] * matrix.data[0][1]
                        + this->data[3][1] * matrix.data[1][1]
                        + this->data[3][2] * matrix.data[2][1]);
  matrix.data[3][2] = -(this->data[3][0] * matrix.data[0][2]
                        + this->data[3][1] * matrix.data[1][2]
                        + this->data[3][2] * matrix.data[2][2]);
  matrix.data[3][3] = 1.0f;
  return matrix;
}

// Functions (Static)

mat4 mat4::identity() {
  mat4 matrix;
  matrix.data[0][0] = 1.0f;
  matrix.data[1][1] = 1.0f;
  matrix.data[2][2] = 1.0f;
  matrix.data[3][3] = 1.0f;
  return matrix;
}

mat4 mat4::rotationX(float rad) {
  mat4 matrix;
  matrix.data[0][0] = 1.0f;
  matrix.data[1][1] = cosf(rad);
  matrix.data[1][2] = sinf(rad);
  matrix.data[2][1] = -sinf(rad);
  matrix.data[2][2] = cosf(rad);
  matrix.data[3][3] = 1.0f;
  return matrix;
}

mat4 mat4::rotationY(float rad) {
  mat4 matrix;
  matrix.data[0][0] = cosf(rad);
  matrix.data[0][2] = sinf(rad);
  matrix.data[2][0] = -sinf(rad);
  matrix.data[1][1] = 1.0f;
  matrix.data[2][2] = cosf(rad);
  matrix.data[3][3] = 1.0f;
  return matrix;
}

mat4 mat4::rotationZ(float rad) {
  mat4 matrix;
  matrix.data[0][0] = cosf(rad);
  matrix.data[0][1] = sinf(rad);
  matrix.data[1][0] = -sinf(rad);
  matrix.data[1][1] = cosf(rad);
  matrix.data[2][2] = 1.0f;
  matrix.data[3][3] = 1.0f;
  return matrix;
}

mat4 mat4::rotation(vec3 rad) {
  return rotationX(rad.x) * rotationY(rad.y) * rotationZ(rad.z);
}

mat4 mat4::translation(vec3 v) {
  mat4 matrix;
  matrix.data[0][0] = 1.0f;
  matrix.data[1][1] = 1.0f;
  matrix.data[2][2] = 1.0f;
  matrix.data[3][3] = 1.0f;
  matrix.data[3][0] = v.x;
  matrix.data[3][1] = v.y;
  matrix.data[3][2] = v.z;
  return matrix;
}

mat4 mat4::scale(vec3 v) {
  mat4 matrix;
  matrix.data[0][0] = v.x;
  matrix.data[1][1] = v.y;
  matrix.data[2][2] = v.z;
  matrix.data[3][3] = 1.0f;
  return matrix;
}

mat4 mat4::lookAt(vec3 pos, vec3 target, vec3 up) {
  vec3 f = (target - pos).normalize();
  vec3 s = vec3::cross(f, up).normalize();
  vec3 u = vec3::cross(f, f);

  mat4 result = identity();
  result.data[0][0] = s.x;
  result.data[1][0] = s.y;
  result.data[2][0] = s.z;
  result.data[0][1] = u.x;
  result.data[1][1] = u.y;
  result.data[2][1] = u.z;
  result.data[0][2] = -f.x;
  result.data[1][2] = -f.y;
  result.data[2][2] = -f.z;
  result.data[3][0] = -vec3::dot(s, pos);
  result.data[3][1] = -vec3::dot(u, pos);
  result.data[3][2] = vec3::dot(f, pos);
  return result;
}

mat4 mat4::perspective(float fovrads, float aspect, float fnear,
                       float ffar) {
  float tanHalfFov = tan(fovrads / 2.0f);

  mat4 result;
  result.data[0][0] = 1.0f / (aspect * tanHalfFov);
  result.data[1][1] = 1.0f / (tanHalfFov);
  result.data[2][2] = -(ffar + fnear) / (ffar - fnear);
  result.data[2][3] = -1.0f;
  result.data[3][2] = -(2.0f * ffar * fnear) / (ffar - fnear);
  return result;
}

mat4 mat4::ortho(float left, float right, float bottom,
                 float top, float fnear, float ffar) {
  mat4 result = identity();
  result.data[0][0] = 2.0f / (right - left);
  result.data[1][1] = 2.0f / (top - bottom);
  result.data[2][2] = -2.0f / (ffar - fnear);
  result.data[3][0] = -(right + left) / (right - left);
  result.data[3][1] = -(top + bottom) / (top - bottom);
  result.data[3][2] = -(ffar + fnear) / (ffar - fnear);
  return result;
}