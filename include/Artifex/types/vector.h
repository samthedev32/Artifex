#pragma once

// TODO: Add ReactPhysics3D Compatibility

// 1D Vector
typedef float vec1;

// 2D Vector
struct vec2 {
    // Data
    union {
        // Position
        struct {
            float x, y;
        };

        // Texture Coordinate
        struct {
            float u, v;
        };

        // Point
        float p[2];
    };

    /* Constructors */

    // vec2(x, y)
    vec2(float x = 0, float y = 0) {
        this->p[0] = x;
        this->p[1] = y;
    }

    // vec2{x, y}
    vec2(float v[2]) { vec2(v[0], v[1]); }

    /* Operator Overloading (vec2) */

    // [+] Operator (vec2)
    vec2 operator+(const vec2 v) const {
        vec2 ret;
        ret.p[0] = this->p[0] + v.p[0];
        ret.p[1] = this->p[1] + v.p[1];
        return ret;
    }

    // [-] Operator (vec2)
    vec2 operator-(const vec2 v) const {
        vec2 ret;
        ret.p[0] = this->p[0] - v.p[0];
        ret.p[1] = this->p[1] - v.p[1];
        return ret;
    }

    // [*] Operator (vec2)
    vec2 operator*(const vec2 v) const {
        vec2 ret;
        ret.p[0] = this->p[0] * v.p[0];
        ret.p[1] = this->p[1] * v.p[1];
        return ret;
    }

    // [/] Operator (vec2)
    vec2 operator/(const vec2 v) const {
        vec2 ret;
        ret.p[0] = this->p[0] / v.p[0];
        ret.p[1] = this->p[1] / v.p[1];
        return ret;
    }

    // [+=] Operator (vec2)
    void operator+=(const vec2 v) {
        this->p[0] += v.p[0];
        this->p[1] += v.p[1];
    }

    // [-=] Operator (vec2)
    void operator-=(const vec2 v) {
        this->p[0] -= v.p[0];
        this->p[1] -= v.p[1];
    }

    // [*=] Operator (vec2)
    void operator*=(const vec2 v) {
        this->p[0] *= v.p[0];
        this->p[1] *= v.p[1];
    }

    // [/=] Operator (vec2)
    void operator/=(const vec2 v) {
        this->p[0] /= v.p[0];
        this->p[1] /= v.p[1];
    }

    // [=] Operator (vec2)
    void operator=(const vec2 v) {
        this->p[0] = v.p[0];
        this->p[1] = v.p[1];
    }

    // [==] Operator (vec2)
    bool operator==(const vec2 v) const {
        return this->p[0] == v.p[0] && this->p[1] == v.p[1];
    }

    // != Operator (vec2)
    bool operator!=(const vec2 v) const {
        return this->p[0] != v.p[0] || this->p[1] != v.p[1];
    }

    /* Operation Overloading (float) */

    // [+] Operator (float)
    vec2 operator+(const float v) const {
        vec2 ret;
        ret.p[0] = this->p[0] + v;
        ret.p[1] = this->p[1] + v;
        return ret;
    }

    // [-] Operator (float)
    vec2 operator-(const float v) const {
        vec2 ret;
        ret.p[0] = this->p[0] - v;
        ret.p[1] = this->p[1] - v;
        return ret;
    }

    // [*] Operator (float)
    vec2 operator*(const float v) const {
        vec2 ret;
        ret.p[0] = this->p[0] * v;
        ret.p[1] = this->p[1] * v;
        return ret;
    }

    // [/] Operator (float)
    vec2 operator/(const float v) const {
        vec2 ret;
        ret.p[0] = this->p[0] / v;
        ret.p[1] = this->p[1] / v;
        return ret;
    }

    // [+=] Operator (float)
    void operator+=(const float v) {
        this->p[0] += v;
        this->p[1] += v;
    }

    // [-=] Operator (float)
    void operator-=(const float v) {
        this->p[0] -= v;
        this->p[1] -= v;
    }

    // [*=] Operator (float)
    void operator*=(const float v) {
        this->p[0] *= v;
        this->p[1] *= v;
    }

    // [/=] Operator (float)
    void operator/=(const float v) {
        this->p[0] /= v;
        this->p[1] /= v;
    }

    // [=] Operator (float)
    void operator=(const float v) {
        this->p[0] = v;
        this->p[1] = v;
    }

    // [==] Operator (float)
    bool operator==(const float v) const {
        return this->p[0] == v && this->p[1] == v;
    }

    // != Operator (float)
    bool operator!=(const float v) const {
        return this->p[0] != v || this->p[1] != v;
    }

    /* Indexing */

    // vec2[id] Get Operator
    float operator[](int i) const { return this->p[i % 2]; }

    // vec2[id] Set Operator
    float &operator[](int i) { return this->p[i % 2]; }

    /* Advanced Functions */

    // The Dot-Product
    float dotproduct(vec2 v) { return this->x * v.x + this->y * v.y; }

    // The Length
    float length() { return sqrtf(dotproduct(*this)); }

    // Distance
    float distance(vec2 v) {
        vec2 dist = *this - v;
        return sqrt(dist.x * dist.x + dist.y * dist.y);
    }

    // Normalize
    vec2 normalize() { return *this / length(); }

    // Create Rotated Movement Vector (this <- effect)
    vec2 rotation(float degrees) {
        vec2 out;

        out.x = sin(rads(degrees));
        out.y = cos(rads(degrees));

        return out * *this;
    }
};

// 3D Vector
struct vec3 {
    // Data
    union {
        // Position
        struct {
            float x, y, z;
        };

        // Color
        struct {
            float r, g, b;
        };

        // Point
        float p[3];
    };

    /* Constructors */

    // vec3(x, y, z)
    vec3(float x = 0, float y = 0, float z = 0) {
        this->p[0] = x;
        this->p[1] = y;
        this->p[2] = z;
    }

    // vec3{x, y, z}
    vec3(float v[3]) { vec3(v[0], v[1], v[2]); }

    /* Operation Overloading (vec3) */

    // [+] Operator (vec2)
    vec3 operator+(const vec3 v) const {
        vec3 ret;
        ret.p[0] = this->p[0] + v.p[0];
        ret.p[1] = this->p[1] + v.p[1];
        ret.p[2] = this->p[2] + v.p[2];
        return ret;
    }

    // [-] Operator (vec3)
    vec3 operator-(const vec3 v) const {
        vec3 ret;
        ret.p[0] = this->p[0] - v.p[0];
        ret.p[1] = this->p[1] - v.p[1];
        ret.p[2] = this->p[2] - v.p[2];
        return ret;
    }

    // [*] Operator (vec3)
    vec3 operator*(const vec3 v) const {
        vec3 ret;
        ret.p[0] = this->p[0] * v.p[0];
        ret.p[1] = this->p[1] * v.p[1];
        ret.p[2] = this->p[2] * v.p[2];
        return ret;
    }

    // [/] Operator (vec3)
    vec3 operator/(const vec3 v) const {
        vec3 ret;
        ret.p[0] = this->p[0] / v.p[0];
        ret.p[1] = this->p[1] / v.p[1];
        ret.p[2] = this->p[2] / v.p[2];
        return ret;
    }

    // [+=] Operator (vec3)
    void operator+=(const vec3 v) {
        this->p[0] += v.p[0];
        this->p[1] += v.p[1];
        this->p[2] += v.p[2];
    }

    // [-=] Operator (vec3)
    void operator-=(const vec3 v) {
        this->p[0] -= v.p[0];
        this->p[1] -= v.p[1];
        this->p[2] -= v.p[2];
    }

    // [*=] Operator (vec3)
    void operator*=(const vec3 v) {
        this->p[0] *= v.p[0];
        this->p[1] *= v.p[1];
        this->p[2] *= v.p[2];
    }

    // [/=] Operator (vec3)
    void operator/=(const vec3 v) {
        this->p[0] /= v.p[0];
        this->p[1] /= v.p[1];
        this->p[2] /= v.p[2];
    }

    // [=] Operator (vec3)
    void operator=(const vec3 v) {
        this->p[0] = v.p[0];
        this->p[1] = v.p[1];
        this->p[2] = v.p[2];
    }

    // [==] Operator (vec3)
    bool operator==(const vec3 v) const {
        return this->p[0] == v.p[0] && this->p[1] == v.p[1] &&
               this->p[2] == v.p[2];
    }

    // != Operator (vec3)
    bool operator!=(const vec3 v) const {
        return this->p[0] != v.p[0] || this->p[1] != v.p[1] ||
               this->p[2] != v.p[2];
    }

    /* Operation Overloading (float) */

    // [+] Operator (float)
    vec3 operator+(const float v) const {
        vec3 ret;
        ret.p[0] = this->p[0] + v;
        ret.p[1] = this->p[1] + v;
        ret.p[2] = this->p[2] + v;
        return ret;
    }

    // [-] Operator (float)
    vec3 operator-(const float v) const {
        vec3 ret;
        ret.p[0] = this->p[0] - v;
        ret.p[1] = this->p[1] - v;
        ret.p[2] = this->p[2] - v;
        return ret;
    }

    // [*] Operator (float)
    vec3 operator*(const float v) const {
        vec3 ret;
        ret.p[0] = this->p[0] * v;
        ret.p[1] = this->p[1] * v;
        ret.p[2] = this->p[2] * v;
        return ret;
    }

    // [/] Operator (float)
    vec3 operator/(const float v) const {
        vec3 ret;
        ret.p[0] = this->p[0] / v;
        ret.p[1] = this->p[1] / v;
        ret.p[2] = this->p[2] / v;
        return ret;
    }

    // [+=] Operator (float)
    void operator+=(const float v) {
        this->p[0] += v;
        this->p[1] += v;
        this->p[2] += v;
    }

    // [-=] Operator (float)
    void operator-=(const float v) {
        this->p[0] -= v;
        this->p[1] -= v;
        this->p[2] -= v;
    }

    // [*=] Operator (float)
    void operator*=(const float v) {
        this->p[0] *= v;
        this->p[1] *= v;
        this->p[2] *= v;
    }

    // [/=] Operator (float)
    void operator/=(const float v) {
        this->p[0] /= v;
        this->p[1] /= v;
        this->p[2] /= v;
    }

    // [=] Operator (float)
    void operator=(const float v) {
        this->p[0] = v;
        this->p[1] = v;
        this->p[2] = v;
    }

    // [==] Operator (float)
    bool operator==(const float v) const {
        return this->p[0] == v && this->p[1] == v && this->p[2] == v;
    }

    // != Operator (float)
    bool operator!=(const float v) const {
        return this->p[0] != v || this->p[1] != v || this->p[2] != v;
    }

    /* Index */

    // vec2[id] Get Operator
    float operator[](int i) const { return this->p[i % 3]; }

    // vec2[id] Set Operator
    float &operator[](int i) { return this->p[i % 3]; }

    /* Advanced Functions */

    // The Dot-Product
    float dotproduct(vec3 v) {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    // The Cross-Product
    vec3 crossproduct(vec3 v) {
        vec3 out;

        out.p[0] = this->y * v.z - this->z * v.y;
        out.p[1] = this->z * v.x - this->x * v.z;
        out.p[2] = this->x * v.y - this->y * v.x;

        return out;
    }

    // The Length
    float length() { return sqrtf(dotproduct(*this)); }

    // Distance between
    float distance(vec3 v) {
        vec3 dist = *this - b;
        return sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
    }

    // Normalize
    vec3 normalize() { return *this / length(); }

    // Calculate the Normal of a Triangle
    vec3 normal(vec3 v[3]) {
        vec3 ret;
        double ux = v[1].x - v[0].x;
        double uy = v[1].y - v[0].y;
        double uz = v[1].z - v[0].z;
        double vx = v[2].x - v[0].x;
        double vy = v[2].y - v[0].y;
        double vz = v[2].z - v[0].z;
        ret.x = uy * vz - uz * vy;
        ret.y = uz * vx - ux * vz;
        ret.z = ux * vy - uy * vx;
        return ret;
    }
};