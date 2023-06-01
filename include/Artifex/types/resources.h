#pragma once

#include <vector>

typedef struct {
    unsigned char *data;
    int width, height;
    int channels;
} Texture;

typedef struct {
} Material;

typedef struct {
    std::vector<float> vertices, texcoords, normals;
    std::vector<int> faces;
} Mesh;