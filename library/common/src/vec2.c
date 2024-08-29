#include <Artifex/vec2.h>
#include <math.h>

vec2 vec2_add(vec2 a, vec2 b) {
    return (vec2){a.x + b.x, a.y + b.y};
}

vec2 vec2_subtract(vec2 a, vec2 b) {
    return (vec2){a.x - b.x, a.y - b.y};
}

vec2 vec2_multiply(vec2 a, vec2 b) {
    return (vec2){a.x * b.x, a.y * b.y};
}

vec2 vec2_divide(vec2 a, vec2 b) {
    return (vec2){a.x / b.x, a.y / b.y};
}

vec2 vec2_scale(vec2 a, float amount) {
    return (vec2){a.x * amount, a.y * amount};
}

vec2 vec2_compare(vec2 a, vec2 b) {
    return (vec2){a.x < b.x ? -1 : (a.x > b.x ? 1 : 0),
                  a.y < b.y ? -1 : (a.y > b.y ? 1 : 0)};
}

AX_VECTOR_TYPE vec2_length(vec2 a) {
    return sqrtf(vec2_dotProduct(a, a));
}

vec2 vec2_normalize(vec2 a) {
    float len = vec2_length(a);
    return vec2_divide(a, (vec2){len, len});
}

AX_VECTOR_TYPE vec2_distance(vec2 a, vec2 b) {
    vec2 dist = vec2_subtract(a, b);
    return sqrt(vec2_dotProduct(dist, dist));
}

AX_VECTOR_TYPE vec2_dotProduct(vec2 a, vec2 b) {
    AX_VECTOR_TYPE out;

    out += a.x * b.x;
    out += a.y * b.y;

    return out;
}

vec2 vec2_lerp(vec2 a, vec2 b, float blend) {
    return (vec2){a.x + (b.x - a.x) * blend, a.y + (b.y - a.y) * blend};
}