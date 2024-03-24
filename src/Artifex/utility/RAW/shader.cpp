#include <Artifex/utility/RAW.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstring>
#include <unordered_map>

namespace RAW {

std::array<std::string, 3> glsl(const char *path, const std::unordered_map<std::string, std::string> &script) {
  if (FILE *f = fopen(path, "r")) {
    int current = -1;

    // RAW Shader Code (vertex, fragment, geometry)
    std::string code[3];

    char line[256];
    while (fgets(line, sizeof(line), f)) {
      char index[10], parameter[10];
      sscanf(line, "%9s %9s", index, parameter);

      if (!strcmp(index, "#shader")) {
        if (!strcmp(parameter, "vertex"))
          current = 0;
        else if (!strcmp(parameter, "fragment"))
          current = 1;
        else if (!strcmp(parameter, "geometry"))
          current = 2;
        else
          printf("Invalid shader type\n");
      } else if (!strcmp(index, "#script")) {
        if (script.count(parameter) != 0 && current != -1)
          code[current] += script.at(parameter);
        else
          printf("missing script\n");
      } else {
        if (current != -1)
          code[current] += line;
      }
    }

    fclose(f);

    return {code[0], code[1], code[2]};
  }

  printf("failed to open file\n");

  // Failed to open file
  return {};
}

bool image(const char *path, std::vector<unsigned char> &data, uint32_t &width, uint32_t &height, uint8_t &channels) {
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  void *image = stbi_load(path, &w, &h, &ch, 3);

  if (!image)
    return false;

  data.resize(w * h * ch);
  memcpy(data.data(), image, w * h * ch);
  free(image);

  return true;
}

} // namespace RAW