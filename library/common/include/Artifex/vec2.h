/**
 * @brief C 2D Vector Library for Artifex
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

// 2D Vector
typedef struct {
    AX_VECTOR_TYPE x, y;
} vec2;

vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_subtract(vec2 a, vec2 b);
vec2 vec2_multiply(vec2 a, vec2 b);
vec2 vec2_divide(vec2 a, vec2 b);

vec2 vec2_scale(vec2 a, float amount);

vec2 vec2_compare(vec2 a, vec2 b);

AX_VECTOR_TYPE vec2_length(vec2 a);
vec2 vec2_normalize(vec2 a);

AX_VECTOR_TYPE vec2_distance(vec2 a, vec2 b);
AX_VECTOR_TYPE vec2_dotProduct(vec2 a, vec2 b);
vec2 vec2_lerp(vec2 a, vec2 b, float blend);