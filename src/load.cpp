#include <GL/load.h>

#include <GL/glad.h>
#include <fstream>

namespace GL::LOAD {

Shader shader(std::string path) {
    bool hasVertex, hasGeometry, hasFragment;
    std::string vertex, geometry, fragment;

    std::ifstream file(path);

    if (!file.is_open()) {
        printf("No Shader File\n");
        return 0;
    }

    std::string line;
    uint current = 0;
    while (getline(file, line)) {
        if (line == "#vertex") {
            current = 1;
            hasVertex = true;
        } else if (line == "#geometry") {
            current = 3;
            hasGeometry = true;
        } else if (line == "#fragment") {
            current = 2;
            hasFragment = true;
        } else {
            // TODO: Process Line
        }
    }

    if (!hasVertex || !hasFragment) {
        printf("Shader has no Vertex/Fragment\n");
        return 0;
    }

    // TODO: Compile & Link Shader
}

Texture texture(std::string path) {
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        printf("No Image Data\n");
        return 0;
    }

    Texture id;

    // Generate Empty Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Select Color Mode
    uint mode;
    switch (nrChannels) {
    default:
    case 0:
    case 3:
        mode = GL_RGB;
        break;

    case 1:
        mode = GL_RED;
        break;

    case 2:
        mode = GL_RG;
        break;

    case 4:
        mode = GL_RGBA;
        break;
    }

    // Create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode,
                 GL_UNSIGNED_BYTE, data);

    // Generate MipMap & Set Parameters
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Free RAW Data
    stbi_image_free(data);

    return id;
}

Material material(std::string path) {
    // TODO
}

Mesh mesh(std::string path) {
    // TODO
}

} // namespace GL::LOAD