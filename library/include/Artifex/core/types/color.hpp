/**
 * @brief C++ Color Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#include <cstdio>
#define AX_COLOR_SUPPORT_CLIB true

#if AX_COLOR_SUPPORT_CLIB
#include "color3.h"
#include "color4.h"
#endif

#include <cmath>
#include <vector>

#ifndef AX_COLOR_TYPE
#define AX_COLOR_TYPE float
#endif

// Color Format
enum class ColorFormat {
    RGB,
    RGBA,
    HSV,
    YUV,
    CMYK,
};

// Color Components
namespace internal {
template <ColorFormat, typename>
struct colorC;
}  // namespace internal

// Variable Dimension Vector
template <ColorFormat F = ColorFormat::RGB, typename T = AX_COLOR_TYPE>
struct color : internal::colorC<F, T> {
    // Constructors & Destructor
    color(std::vector<T>);

    template <typename... Args>
    color(Args...);

#if AX_COLOR_SUPPORT_CLIB
    color(color3, ColorFormat);
    color(color4, ColorFormat);
#endif

    ~color() = default;

    // Assignment Operators

    template <ColorFormat inF, typename inT>
    color<F, T>& operator=(const color<inF, inT>& v);
    color<F, T>& operator=(const T& v);

    // Relational Operators

    template <ColorFormat inF, typename inT>
    bool operator==(const color<inF, inT>& v) const;

    template <ColorFormat inF, typename inT>
    bool operator!=(const color<inF, inT>& v) const;

    template <ColorFormat inF, typename inT>
    bool operator>(const color<inF, inT>& v) const;

    template <ColorFormat inF, typename inT>
    bool operator<(const color<inF, inT>& v) const;

    template <ColorFormat inF, typename inT>
    bool operator>=(const color<inF, inT>& v) const;

    template <ColorFormat inF, typename inT>
    bool operator<=(const color<inF, inT>& v) const;

#if AX_COLOR_SUPPORT_CLIB
    operator color3() const;
    operator color4() const;
#endif
};

namespace internal {
template <typename T>
struct colorC<ColorFormat::RGB, T> {
    union {
        struct {
            T r, g, b;
        };
        T data[3];
    };
};

template <typename T>
struct colorC<ColorFormat::RGBA, T> {
    union {
        struct {
            T r, g, b, a;
        };
        T data[4];
    };
};

template <typename T>
struct colorC<ColorFormat::HSV, T> {
    union {
        struct {
            T h, s, v;
        };
        T data[3];
    };
};

template <typename T>
struct colorC<ColorFormat::YUV, T> {
    union {
        struct {
            T y, u, v;
        };
        T data[3];
    };
};

template <typename T>
struct colorC<ColorFormat::CMYK, T> {
    union {
        struct {
            T c, m, y, k;
        };
        T data[4];
    };
};
}  // namespace internal

// Constructors & Destructor

template <ColorFormat F, typename T>
color<F, T>::color(std::vector<T> v) {
    for (int i = 0; i < sizeof(this->data); i++)
        this->data[i] = i < v.size() ? v[i] : 0;
}

template <ColorFormat F, typename T>
template <typename... Args>
color<F, T>::color(Args... args) {
    T values[] = {static_cast<T>(args)...};
    for (int i = 0; i < sizeof(this->data); i++)
        this->data[i] = sizeof...(args) == 1 ? values[0] : (i < sizeof...(args) ? values[i] : 0);
}

#if AX_COLOR_SUPPORT_CLIB
template <ColorFormat F, typename T>
color<F, T>::color(color3 c, ColorFormat f) {
    // *this = color<f, T>(c.data);
}

template <ColorFormat F, typename T>
color<F, T>::color(color4 c, ColorFormat f) {
    // *this = color<f, T>(c.data);
}
#endif

/*
template <vec_t D, typename T>
template <vec_t inD, typename inT>
vec<D, T>::vec(vec<inD, inT> v) { *this = v; }

#if AX_COLOR_SUPPORT_CLIB
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

#if AX_COLOR_SUPPORT_CLIB
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
*/