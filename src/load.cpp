#include <Artifex/load.h>

#include <GL/glad.h>
#include <cstring>
#include <fstream>
#include <random>

namespace Artifex {

ResourceManager::ResourceManager() {
    // TODO: Init Resource Loader
}

ResourceManager::~ResourceManager() {
    // Free Textures
    for (Texture tex : textures)
        free(tex.data);

    textures.clear();

    // Free Materials
    // for (Material mat : materials)

    materials.clear();

    // Free Meshes
    for (Mesh mesh : meshes) {
        mesh.vertices.clear();
        mesh.texcoords.clear();
        mesh.normals.clear();

        mesh.faces.clear();
    }

    meshes.clear();
}

unsigned int ResourceManager::load_texture(std::string path) {
    Texture out;

    out.data =
        stbi_load(path.c_str(), &out.width, &out.height, &out.channels, 0);

    // TODO

    return 0;
}

unsigned int ResourceManager::load_material(std::string path) {
    // TODO
    return 0;
}

unsigned int ResourceManager::load_mesh(std::string path) {
    // TODO
    return 0;
}

} // namespace Artifex