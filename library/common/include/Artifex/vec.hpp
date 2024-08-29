/**
 * @brief C++ Vector, Color & UV Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#define AX_VECTOR_SUPPORT_CLIB true

#if AX_VECTOR_SUPPORT_CLIB
#include <Artifex/color.h>
#include <Artifex/vec2.h>
#include <Artifex/vec3.h>
#endif

#include <cmath>
#include <cstdint>
#include <vector>

// Vector Size Type
typedef uint8_t vec_t;

// Vector Components
namespace internal {
template <vec_t D, typename T>
struct vecC;
}  // namespace internal

// Variable Dimension Vector
template <vec_t D = 3, typename T = AX_VECTOR_TYPE>
struct vec {
    static_assert(D != 0, "Null-Size Vectors are not supported");

    union {
        T data[D];

        internal::vecC<D, T> component;
    };

    // Constructors & Destructor

    vec(std::vector<T> v);

    template <typename... Args>
    vec(Args... args);

    template <vec_t inD, typename inT>
    vec(vec<inD, inT> v);

#if AX_VECTOR_SUPPORT_CLIB
    vec(vec2 v);
    vec(vec3 v);

    vec(axColor c);
#endif

    ~vec();

    // Arithmetic Operators

    template <vec_t inD, typename inT>
    vec<D, T> operator+(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    vec<D, T> operator-(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    vec<D, T> operator*(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    vec<D, T> operator/(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    vec<D, T> operator%(const vec<inD, inT>& v) const;

    vec<D, T> operator*(const T& v) const;

    vec<D, T> operator/(const T& v) const;

    vec<D, T> operator%(const T& v) const;

    // Assignment Operators

    template <vec_t inD, typename inT>
    vec<D, T>& operator=(const vec<inD, inT>& v);

    template <vec_t inD, typename inT>
    void operator+=(const vec<inD, inT>& v);

    template <vec_t inD, typename inT>
    void operator-=(const vec<inD, inT>& v);

    template <vec_t inD, typename inT>
    void operator*=(const vec<inD, inT>& v);

    template <vec_t inD, typename inT>
    void operator/=(const vec<inD, inT>& v);

    template <vec_t inD, typename inT>
    void operator%=(const vec<inD, inT>& v);

    vec<D, T>& operator=(const T& v);

    void operator*=(const T& v);

    void operator/=(const T& v);

    void operator%=(const T& v);

    // Relational Operators

    template <vec_t inD, typename inT>
    bool operator==(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    bool operator!=(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    bool operator>(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    bool operator<(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    bool operator>=(const vec<inD, inT>& v) const;

    template <vec_t inD, typename inT>
    bool operator<=(const vec<inD, inT>& v) const;

    // Other Operators

    T operator[](vec_t i) const;

    T& operator[](vec_t i);

    void operator++();

    void operator--();

    internal::vecC<D, T>* operator->() { return &component; }
    const internal::vecC<D, T>* operator->() const { return &component; }

#if AX_VECTOR_SUPPORT_CLIB
    operator vec2() const;
    operator vec3() const;
    operator axColor() const;
#endif

    // Functions (Instance Methods)

    [[nodiscard]] T length() const;

    [[nodiscard]] vec normalize() const;

    [[nodiscard]] vec clamp(vec min, vec max) const;

    // Functions (Static)

    static T distance(vec a, vec b);

    static T dot(vec a, vec b);

    static vec lerp(vec a, vec b, T blend);

    // Functions (Dimension-Specific)

    static vec<3> cross(const vec<3>& a, const vec<3>& b);
};

namespace internal {
// 1D Vector Component
template <typename T>
struct vecC<1, T> {
    T x;
};

// 2D Vector Components
template <typename T>
struct vecC<2, T> {
    union {
        // Position
        struct {
            T x, y;
        };

        // TexCoord
        struct {
            T u, v;
        };

        // Size
        struct {
            T width, height;
        };
    };
};

// 3D Vector Components
template <typename T>
struct vecC<3, T> {
    union {
        // Position
        struct {
            T x, y, z;
        };

        // TexCoord
        struct {
            T u, v, w;
        };

        // Color
        struct {
            T red, green, blue;
        };

        // Size
        struct {
            T width, height, depth;
        };
    };
};

// 4D Vector Components
template <typename T>
struct vecC<4, T> {
    union {
        // Position
        struct {
            T x, y, z, w;
        };

        // Color
        struct {
            T red, green, blue, alpha;
        };
    };
};
}  // namespace internal

// Constructors & Destructor

template <vec_t D, typename T>
vec<D, T>::vec(std::vector<T> v) {
    for (vec_t i = 0; i < D; i++)
        this->data[i] = i < v.size() ? v[i] : 0;
}

template <vec_t D, typename T>
template <typename... Args>
vec<D, T>::vec(Args... args) {
    T values[] = {static_cast<T>(args)...};
    for (vec_t i = 0; i < D; i++)
        this->data[i] = sizeof...(args) == 1 ? values[0] : (i < sizeof...(args) ? values[i] : 0);
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T>::vec(vec<inD, inT> v) { *this = v; }

#if AX_VECTOR_SUPPORT_CLIB
template <vec_t D, typename T>
vec<D, T>::vec(vec2 v) {
    *this = vec(v.x, v.y);
}

template <vec_t D, typename T>
vec<D, T>::vec(vec3 v) {
    *this = vec(v.x, v.y, v.z);
}

template <vec_t D, typename T>
vec<D, T>::vec(axColor c) {
    *this = vec(c.r, c.g, c.b);
}
#endif

template <vec_t D, typename T>
vec<D, T>::~vec() {
}

// Arithmetic Operators

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T> vec<D, T>::operator+(const vec<inD, inT>& v) const {
    vec<D, T> ret;
    for (vec_t i = 0; i < std::min(D, inD); i++)
        ret.data[i] = this->data[i] + v.data[i];
    return ret;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T> vec<D, T>::operator-(const vec<inD, inT>& v) const {
    vec<D, T> ret;
    for (vec_t i = 0; i < std::min(D, inD); i++)
        ret.data[i] = this->data[i] - v.data[i];
    return ret;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T> vec<D, T>::operator*(const vec<inD, inT>& v) const {
    vec<D, T> ret;
    for (vec_t i = 0; i < std::min(D, inD); i++)
        ret.data[i] = this->data[i] * v.data[i];
    return ret;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T> vec<D, T>::operator/(const vec<inD, inT>& v) const {
    vec<D, T> ret;
    for (vec_t i = 0; i < std::min(D, inD); i++)
        ret.data[i] = this->data[i] / v.data[i];
    return ret;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T> vec<D, T>::operator%(const vec<inD, inT>& v) const {
    vec<D, T> ret;
    for (vec_t i = 0; i < std::min(D, inD); i++)
        ret.data[i] = this->data[i] % v.data[i];
    return ret;
}

template <vec_t D, typename T>
vec<D, T> vec<D, T>::operator*(const T& v) const {
    vec<D, T> out;
    for (vec_t i = 0; i < D; i++)
        out.data[i] = this->data[i] * v;
    return out;
}

template <vec_t D, typename T>
vec<D, T> vec<D, T>::operator/(const T& v) const {
    vec<D, T> out;
    for (vec_t i = 0; i < D; i++)
        out.data[i] = this->data[i] / v;
    return out;
}

template <vec_t D, typename T>
vec<D, T> vec<D, T>::operator%(const T& v) const {
    vec<D, T> out;
    for (vec_t i = 0; i < D; i++)
        out.data[i] = this->data[i] % v;
    return out;
}

// Assignment Operators

template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T>& vec<D, T>::operator=(const vec<inD, inT>& v) {
    for (vec_t i = 0; i < D; i++)
        this->data[i] = i < inD ? v.data[i] : 0;

    return *this;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
void vec<D, T>::operator+=(const vec<inD, inT>& v) {
    *this = *this + v;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
void vec<D, T>::operator-=(const vec<inD, inT>& v) {
    *this = *this - v;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
void vec<D, T>::operator*=(const vec<inD, inT>& v) {
    *this = *this * v;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
void vec<D, T>::operator/=(const vec<inD, inT>& v) {
    *this = *this / v;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
void vec<D, T>::operator%=(const vec<inD, inT>& v) {
    *this = *this % v;
}

template <vec_t D, typename T>
vec<D, T>& vec<D, T>::operator=(const T& v) {
    for (vec_t i = 0; i < D; i++)
        this->data[i] = v;

    return *this;
}

template <vec_t D, typename T>
void vec<D, T>::operator*=(const T& v) { *this = *this * v; }

template <vec_t D, typename T>
void vec<D, T>::operator/=(const T& v) { *this = *this / v; }

template <vec_t D, typename T>
void vec<D, T>::operator%=(const T& v) { *this = *this % v; }

// Relational Operators

template <vec_t D, typename T>
template <vec_t inD, typename inT>
bool vec<D, T>::operator==(const vec<inD, inT>& v) const {
    for (vec_t i = 0; i < D; i++)
        if (this->data[i] != v.data[i])
            return false;
    return true;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
bool vec<D, T>::operator!=(const vec<inD, inT>& v) const {
    for (vec_t i = 0; i < D; i++)
        if (this->data[i] == v.data[i])
            return false;
    return true;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
bool vec<D, T>::operator>(const vec<inD, inT>& v) const {
    // TODO
    return false;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
bool vec<D, T>::operator<(const vec<inD, inT>& v) const {
    // TODO
    return false;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
bool vec<D, T>::operator>=(const vec<inD, inT>& v) const {
    // TODO
    return false;
}

template <vec_t D, typename T>
template <vec_t inD, typename inT>
bool vec<D, T>::operator<=(const vec<inD, inT>& v) const {
    // TODO
    return false;
}

// Other Operators

template <vec_t D, typename T>
T vec<D, T>::operator[](vec_t i) const { return this->data[i % D]; }

template <vec_t D, typename T>
T& vec<D, T>::operator[](vec_t i) { return this->data[i % D]; }

template <vec_t D, typename T>
void vec<D, T>::operator++() { *this += 1.0f; }

template <vec_t D, typename T>
void vec<D, T>::operator--() { *this -= 1.0f; }

#if AX_VECTOR_SUPPORT_CLIB
template <vec_t D, typename T>
vec<D, T>::operator vec2() const {
    return (vec2){data[0], data[D < 2 ? 1 : 0]};
}

template <vec_t D, typename T>
vec<D, T>::operator vec3() const {
    return (vec3){data[0], data[D < 2 ? 1 : 0], D < 3 ? data[2] : 0};
}

template <vec_t D, typename T>
vec<D, T>::operator axColor() const {
    return (axColor){data[0], data[D < 2 ? 0 : 1], D == 2 ? 0 : data[D < 2 ? 0 : 2]};
}
#endif

// Functions (Instance Methods)

template <vec_t D, typename T>
T vec<D, T>::length() const { return sqrtf(dot(*this, *this)); }

template <vec_t D, typename T>
vec<D, T> vec<D, T>::normalize() const { return *this / this->length(); }

template <vec_t D, typename T>
vec<D, T> vec<D, T>::clamp(vec<D, T> min, vec<D, T> max) const {
    vec<D, T> out;

    for (vec_t i = 0; i < D; i++) {
        if (this->data[i] < min.data[i])
            out.data[i] = min.data[i];
        else if (this->data[i] > max.data[i])
            out.data[i] = max.data[i];
        else
            out.data[i] = this->data[i];
    }

    return out;
}

// Functions (Static)

template <vec_t D, typename T>
T vec<D, T>::distance(vec a, vec b) {
    vec<D, T> dist = a - b;
    return sqrt(dot(dist, dist));
}

template <vec_t D, typename T>
T vec<D, T>::dot(vec a, vec b) {
    T out;

    for (vec_t i = 0; i < D; i++)
        out += a[i] * b[i];

    return out;
}

template <vec_t D, typename T>
vec<D, T> vec<D, T>::lerp(vec<D, T> a, vec<D, T> b, T blend) {
    vec<D, T> out;

    for (vec_t i = 0; i < D; i++)
        out[i] = a.data[i] + (b.data[i] - a.data[i]) * blend;

    return out;
}

// Functions (Dimension-Specific)

template <vec_t D, typename T>
vec<3> vec<D, T>::cross(const vec<3>& a, const vec<3>& b) {
    return {a.data[1] * b.data[2] - a.data[2] * b.data[1], a.data[2] * b.data[0] - a.data[0] * b.data[2],
            a.data[0] * b.data[1] - a.data[1] * b.data[0]};
}