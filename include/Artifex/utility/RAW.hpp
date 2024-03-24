#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstdint>

// RAW File Loader
namespace RAW {

// Load GLSL Shader File
std::array<std::string, 3> glsl(const char *path, const std::unordered_map<std::string, std::string> &script = {});

// Load Image
bool image(const char *path, std::vector<unsigned char> &data, uint32_t &width, uint32_t &height, uint8_t &channels);

// Load Wavefront OBJ
// void obj(const char *path);

} // namespace RAW