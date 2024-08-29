/**
 * @brief C 3D Vector Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Vector Component Type
#ifndef AX_VECTOR_TYPE
#define AX_VECTOR_TYPE float
#endif

// 3D Vector
typedef struct {
    AX_VECTOR_TYPE x, y, z;
} vec3;

vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_subtract(vec3 a, vec3 b);
vec3 vec3_multiply(vec3 a, vec3 b);
vec3 vec3_divide(vec3 a, vec3 b);

vec3 vec3_scale(vec3 a, float amount);

vec3 vec3_compare(vec3 a, vec3 b);

AX_VECTOR_TYPE vec3_length(vec3 a);
vec3 vec3_normalize(vec3 a);

AX_VECTOR_TYPE vec3_distance(vec3 a, vec3 b);
AX_VECTOR_TYPE vec3_dotProduct(vec3 a, vec3 b);
vec3 vec3_crossProduct(vec3 a, vec3 b);
vec3 vec3_lerp(vec3 a, vec3 b, float blend);