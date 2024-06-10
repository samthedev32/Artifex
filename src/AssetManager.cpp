#include <Artifex/AssetManager.hpp>
#include <Artifex/Renderer.hpp>
#include <Artifex/Mixer.hpp>

#include <Artifex/Log.hpp>

#include <stdexcept>

#include <GL/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stb_vorbis.h>
#include <unordered_map>

#include <cstring>

#define TAG "AssetManager"

// TODO debug messages

namespace Artifex {
    AssetManager::AssetManager(Renderer &renderer, Mixer &mixer) : m_renderer(renderer), m_mixer(mixer) {
        // Config Loaders
        stbi_set_flip_vertically_on_load(true);
    }

    AssetManager::~AssetManager() {
        m_renderer.use();
        m_mixer.use();

        // Unload all assets
        for (auto [id, _]: texture)
            unload(id);

        for (auto [id, _]: audio)
            unload(id);

        for (auto [id, _]: scene)
            unload(id);

        texture.clear();
        audio.clear();
        scene.clear();
    }

    UUID AssetManager::add(const char *vertex, const char *fragment, const char *geometry) {
        UUID uuid = UUID(Type::Shader);
        shader[uuid] = m_renderer.load(vertex, fragment, geometry);
        return uuid;
    }

    UUID AssetManager::add(int width, int height, int channels, const unsigned char *data) {
        UUID uuid = UUID(Type::Texture);
        texture[uuid] = m_renderer.load(width, height, channels, data);
        return uuid;
    }

    UUID AssetManager::add(const vec<2> *vertices, size_t v_size, const uint32_t *indices, size_t i_size) {
        UUID uuid = UUID(Type::Mesh);
        mesh[uuid] = m_renderer.load(vertices, v_size, indices, i_size);
        return uuid;
    }

    UUID AssetManager::add(int channels, int sample_rate, int samples, short *data) {
        UUID uuid = UUID(Type::Audio);
        texture[uuid] = m_mixer.load(channels, sample_rate, samples, data);
        return uuid;
    }

    UUID AssetManager::load(Type type, const std::string &path) {
        switch (type) {
            case Type::Shader: {
                // ---- Load from file
                std::unordered_map<std::string, std::string> script;

                if (FILE *f = fopen(path.c_str(), "r")) {
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

                    return add(code[0].c_str(), code[1].c_str(), code[2].c_str());
                }

                Log::error(TAG, "load (shader): failed to open file");
                return nil;
            }

            case Type::Texture: {
                // ---- Load from file
                int w, h, ch;
                const unsigned char *data = stbi_load(path.c_str(), &w, &h, &ch, 0);

                // Failed to load
                if (!data || w < 1 || h < 1 || ch < 1 || ch > 4) {
                    Log::error(TAG, "load (texture): failed to open file");
                    return nil;
                }

                UUID id = add(w, h, ch, data);
                free((void *) data);
                return id;
            }

            case Type::Mesh: {
                // ---- Load from file
                const vec<2> *vertices{};
                size_t v_size{};

                const uint32_t *indices{};
                size_t i_size{};

                // TODO load
                Log::error(TAG, "load (mesh): not yet implemented");

                UUID id = add(vertices, v_size, indices, i_size);
                free((void *) vertices);
                free((void *) indices);
                return id;
            }

            case Type::Audio: {
                // ---- Load from file
                int channels, sample_rate;
                short *data; // stb_vorbis loads 16 bit audio
                int samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sample_rate, &data);

                if (!data || channels < 1 || sample_rate < 1 || samples < 1) {
                    Log::error(TAG, "load (audio): failed to open file");
                    return nil;
                }

                UUID id = add(channels, sample_rate, samples, data);
                free((void *) data);
                return id;
            }

            case Type::Scene: {
                // TODO
                Log::error(TAG, "load (scene): not yet implemented");
                return nil;
            }

            default:
                // TODO invalid type
                Log::warning(TAG, "load: invalid type");
                return nil;
        }
    }

    void AssetManager::unload(UUID id) {
        switch (getType(id)) {
            case Type::Shader: {
                if (!shader.count(id))
                    return;

                m_renderer.unload_shader(shader[id]);
                shader.erase(id);
            } break;

            case Type::Texture: {
                if (!texture.count(id))
                    return;

                m_renderer.unload(texture[id]);
                texture.erase(id);
            } break;

            case Type::Mesh: {
                if (!mesh.count(id))
                    return;

                m_renderer.unload(mesh[id]);
                mesh.erase(id);
            } break;

            case Type::Audio: {
                if (!audio.count(id))
                    return;

                m_mixer.unload(audio[id]);
                audio.erase(id);
            } break;

            case Type::Scene: {
                // TODO
            } break;

            default:
                // TODO invalid type
                break;
        }
    }

    bool AssetManager::isReady(UUID id) {
        // TODO return async state
        return true;
    }

    AssetManager::Type AssetManager::getType(UUID id) {
        return Type(id.system);
    }
}
