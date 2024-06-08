#include <Artifex/AssetManager.hpp>
#include <Artifex/Window.hpp>
#include <Artifex/Mixer.hpp>

#include <Artifex/Log.hpp>

#include <stdexcept>

#include <GL/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <stb_vorbis.h>
#include <unordered_map>

#include <cstring>

#define TAG "AssetManager"

// TODO debug messages

namespace Artifex {
    AssetManager::AssetManager(Window &window, Mixer &mixer) : m_window(window), m_mixer(mixer) {
        // Config Loaders
        stbi_set_flip_vertically_on_load(true);
    }

    AssetManager::~AssetManager() {
        m_window.use();
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
        m_window.use();

        constexpr size_t minSize = 8;
        if (!vertex || !fragment || strlen(vertex) < minSize || strlen(fragment) < minSize) {
            Log::error(TAG, "add (shader): invalid resources");
            return nil;
        }

        bool isGeo = geometry != nullptr && strlen(geometry) >= minSize;

        // ReSharper disable once CppDFAUnusedValue
        GLuint geo = 0;

        int success;
        char infoLog[1024];

        // ---- Compile Vertex Shader
        const GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vertex, nullptr);
        glCompileShader(vert);

        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vert, 1024, nullptr, infoLog);
            Log::error(TAG, "failed to compile Vertex Shader:\n%s", infoLog);
            return nil;
        }

        // ---- Compile Fragment Shader
        const GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fragment, nullptr);
        glCompileShader(frag);

        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag, 1024, nullptr, infoLog);
            glDeleteShader(vert);
            printf("Failed to compile Fragment Shader:\n%s", infoLog);
            Log::error(TAG, "failed to compile Fragment Shader:\n%s", infoLog);
            // TODO error
            return nil;
        }

        // ---- Compile Geometry Shader (if present)
        if (isGeo) {
            geo = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(geo, 1, &geometry, nullptr);
            glCompileShader(geo);

            glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(geo, 1024, nullptr, infoLog);
                printf("Failed to compile Geometry Shader:\n%s", infoLog);
                Log::warning(TAG, "failed to compile Geometry Shader:\n%s", infoLog);
                // TODO warning
                isGeo = false;
            }
        }

        // ---- Create Shader Program
        GLuint id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        if (isGeo)
            glAttachShader(id, geo);

        glLinkProgram(id);

        // Delete unnecessary shaders
        glDeleteShader(vert);
        glDeleteShader(frag);
        if (isGeo)
            glDeleteShader(geo);

        // Check if linking was successful
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            printf("Failed to Link Shaders:\n%s", infoLog);
            Log::error(TAG, "failed to link shaders");
            return nil;
        }

        // Add to list + return ID
        UUID uuid = UUID(Type::Shader);
        shader[uuid] = id;
        return uuid;
    }

    UUID AssetManager::add(int width, int height, int channels, const unsigned char *data) {
        m_window.use();

        if (width < 1 || height < 1 || channels < 1 || channels > 4 || !data) {
            Log::error(TAG, "add (texture): invalid resources");
            return nil;
        }

        GLuint id;
        // Generate Empty Texture
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // TODO verify resources

        // Select Color Mode
        GLint mode;
        switch (channels) {
            case 1:
                mode = GL_RED;
                break;

            case 2:
                mode = GL_RG;
                break;

            default:
            case 3:
                mode = GL_RGB;
                break;

            case 4:
                mode = GL_RGBA;
                break;
        }

        // Create Texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);

        // Generate MipMap & Set Parameters
        // glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Add to list + return ID
        UUID uuid = UUID(Type::Texture);
        texture[uuid] = id;
        return uuid;
    }

    UUID AssetManager::add(const vec<2> *vertices, size_t v_size, const uint32_t *indices, size_t i_size) {
        m_window.use();

        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // TODO verify resources

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, v_size * sizeof(*vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size * sizeof(*indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Add to list + return ID
        UUID uuid = UUID(Type::Mesh);
        mesh[uuid] = {VAO, VBO, EBO};
        return uuid;
    }

    UUID AssetManager::add(int channels, int sample_rate, int samples, short *data) {
        m_mixer.use();

        if (channels < 1 || channels > 2 || sample_rate < 1 || samples < 1 || !data) {
            Log::error(TAG, "add (audio): invalid resources");
            return nil;
        }

        // Size of audio data in bytes
        const ALsizei size = channels * sizeof(short) * samples;

        // Generate buffer and source
        ALuint id;
        alGenBuffers(1, &id);

        // TODO verify resources

        // Load audio data into buffer
        alBufferData(id, channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, size, sample_rate);

        // Add to list + return ID
        UUID uuid = UUID(Type::Audio);
        texture[uuid] = id;
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

                m_window.use();
                glDeleteShader(shader[id]);
                shader.erase(id);
            } break;

            case Type::Texture: {
                if (!texture.count(id))
                    return;

                m_window.use();
                glDeleteTextures(1, &texture[id]);
                texture.erase(id);
            } break;

            case Type::Mesh: {
                if (!mesh.count(id))
                    return;

                m_window.use();
                auto m = mesh[id];

                glDeleteBuffers(1, &m.VBO);
                glDeleteBuffers(1, &m.EBO);
                glDeleteVertexArrays(1, &m.VAO);
                mesh.erase(id);
            } break;

            case Type::Audio: {
                if (!audio.count(id))
                    return;

                m_mixer.use();
                alDeleteBuffers(1, &audio[id]);
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
