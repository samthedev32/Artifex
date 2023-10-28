#include <Artifex/math/mat/mat3.hpp>

// Constructor & Destructor

mat3::mat3() {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      this->data[i][j] = 0.0f;
}
mat3::~mat3() {}

// Arithmetic Operators

mat3 mat3::operator*(const mat3 m) {
  mat3 matrix;
  for (int c = 0; c < 3; c++)
    for (int r = 0; r < 3; r++)
      matrix.data[r][c] = this->data[r][0] * m.data[0][c]
                          + this->data[r][1] * m.data[1][c]
                          + this->data[r][2] * m.data[2][c];
  // TODO: validate
  return matrix;
}

vec3 mat3::operator*(const vec3 v) {
  return vec3(
      v.x * this->data[0][0] + v.y * this->data[1][0]
          + v.z * this->data[2][0] + 1 * this->data[3][0],
      v.x * this->data[0][1] + v.y * this->data[1][1]
          + v.z * this->data[2][1] + 1 * this->data[3][1],
      v.x * this->data[0][2] + v.y * this->data[1][2]
          + v.z * this->data[2][2] + 1 * this->data[3][2]);
}

// Assignment Operators

void mat3::operator=(const mat3 m) {
  for (int c = 0; c < 3; c++)
    for (int r = 0; r < 3; r++)
      this->data[r][c] = m.data[r][c];
}

// Relational Operators

bool mat3::operator==(const mat3 &m) const {
  for (int c = 0; c < 3; c++)
    for (int r = 0; r < 3; r++)
      if (this->data[r][c] != m.data[r][c])
        return false;
  return true;
}

bool mat3::operator!=(const mat3 &m) const {
  for (int c = 0; c < 3; c++)
    for (int r = 0; r < 3; r++)
      if (this->data[r][c] == m.data[r][c])
        return false;
  return true;
}

// Other Operators

float mat3::operator()(uint8_t x, uint8_t y) const {
  return this->data[x % 3][y % 3];
}

float &mat3::operator()(uint8_t x, uint8_t y) {
  return this->data[x % 3][y % 3];
}

// Functions (Instance Methods)

mat3 mat3::inverse() const {
  mat3 matrix;
  // matrix.data[0][0] = this->data[0][0];
  // matrix.data[0][1] = this->data[1][0];
  // matrix.data[0][2] = this->data[2][0];
  // matrix.data[0][3] = 0.0f;
  // matrix.data[1][0] = this->data[0][1];
  // matrix.data[1][1] = this->data[1][1];
  // matrix.data[1][2] = this->data[2][1];
  // matrix.data[1][3] = 0.0f;
  // matrix.data[2][0] = this->data[0][2];
  // matrix.data[2][1] = this->data[1][2];
  // matrix.data[2][2] = this->data[2][2];
  // matrix.data[2][3] = 0.0f;
  // matrix.data[3][0] = -(this->data[3][0] * matrix.data[0][0]
  // +
  //                       this->data[3][1] * matrix.data[1][0]
  //                       + this->data[3][2] *
  //                       matrix.data[2][0]);
  // matrix.data[3][1] = -(this->data[3][0] * matrix.data[0][1]
  // +
  //                       this->data[3][1] * matrix.data[1][1]
  //                       + this->data[3][2] *
  //                       matrix.data[2][1]);
  // matrix.data[3][2] = -(this->data[3][0] * matrix.data[0][2]
  // +
  //                       this->data[3][1] * matrix.data[1][2]
  //                       + this->data[3][2] *
  //                       matrix.data[2][2]);
  // matrix.data[3][3] = 1.0f;
  return matrix;
}

// Functions (Static)

mat3 mat3::identity() {
  mat3 matrix;
  matrix.data[0][0] = 1.0f;
  matrix.data[1][1] = 1.0f;
  matrix.data[2][2] = 1.0f;
  return matrix;
}

mat3 mat3::rotationX(float rad) {
  mat3 matrix;
  matrix.data[0][0] = 1.0f;
  matrix.data[1][1] = cosf(rad);
  matrix.data[1][2] = sinf(rad);
  matrix.data[2][1] = -sinf(rad);
  matrix.data[2][2] = cosf(rad);
  return matrix;
}

mat3 mat3::rotationY(float rad) {
  mat3 matrix;
  matrix.data[0][0] = cosf(rad);
  matrix.data[0][2] = sinf(rad);
  matrix.data[2][0] = -sinf(rad);
  matrix.data[1][1] = 1.0f;
  matrix.data[2][2] = cosf(rad);
  return matrix;
}

mat3 mat3::rotationZ(float rad) {
  mat3 matrix;
  matrix.data[0][0] = cosf(rad);
  matrix.data[0][1] = sinf(rad);
  matrix.data[1][0] = -sinf(rad);
  matrix.data[1][1] = cosf(rad);
  matrix.data[2][2] = 1.0f;
  return matrix;
}

mat3 mat3::rotation(vec3 rad) {
  return rotationX(rad.x) * rotationY(rad.y) * rotationZ(rad.z);
}

mat3 mat3::translation(vec3 v) {
  mat3 matrix;
  matrix.data[0][0] = 1.0f;
  matrix.data[1][1] = 1.0f;
  matrix.data[2][2] = 1.0f;
  return matrix;
}

mat3 mat3::scale(vec3 v) {
  mat3 matrix;
  matrix.data[0][0] = v.x;
  matrix.data[1][1] = v.y;
  matrix.data[2][2] = v.z;
  return matrix;
}