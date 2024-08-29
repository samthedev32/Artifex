#include <Artifex/vec3.h>
#include <math.h>

vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3_subtract(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec3_multiply(vec3 a, vec3 b) {
    return (vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

vec3 vec3_divide(vec3 a, vec3 b) {
    return (vec3){a.x / b.x, a.y / b.y, a.z / b.z};
}

vec3 vec3_scale(vec3 a, float amount) {
    return (vec3){a.x * amount, a.y * amount, a.z * amount};
}

vec3 vec3_compare(vec3 a, vec3 b) {
    return (vec3){
        a.x < b.x ? -1 : (a.x > b.x ? 1 : 0),
        a.y < b.y ? -1 : (a.y > b.y ? 1 : 0),
        a.z < b.z ? -1 : (a.z > b.z ? 1 : 0)};
}

AX_VECTOR_TYPE vec3_length(vec3 a) {
    return sqrtf(vec3_dotProduct(a, a));
}

vec3 vec3_normalize(vec3 a) {
    float len = vec3_length(a);
    return vec3_divide(a, (vec3){len, len, len});
}

AX_VECTOR_TYPE vec3_distance(vec3 a, vec3 b) {
    vec3 dist = vec3_subtract(a, b);
    return sqrt(vec3_dotProduct(dist, dist));
}

AX_VECTOR_TYPE vec3_dotProduct(vec3 a, vec3 b) {
    AX_VECTOR_TYPE out;

    out += a.x * b.x;
    out += a.y * b.y;
    out += a.z * b.z;

    return out;
}

vec3 vec3_crossProduct(vec3 a, vec3 b) {
    return (vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x};
}

vec3 vec3_lerp(vec3 a, vec3 b, float blend) {
    return (vec3){a.x + (b.x - a.x) * blend, a.y + (b.y - a.y) * blend, a.z + (b.z - a.z) * blend};
}