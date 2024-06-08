#pragma once

#include <Artifex/types/uuid.hpp>
#include <Artifex/types/vec.hpp>

#include <string>
#include <map>
#include <set>

namespace Artifex {
    class Window;

    class Mixer;

    struct Scene {
        // TODO world config stuff

        std::set<UUID> shader, texture, audio;
    };

    struct SceneDescriptor {
        // TODO path to resources, entities, etc
    };

    struct mesh_t {
        unsigned int VAO, VBO, EBO;
    };

    class AssetManager {
    public:
        AssetManager(Window &window, Mixer &mixer);

        ~AssetManager();

        // Resource Type
        enum Type {
            // Graphics
            Shader,
            Texture,
            Mesh,

            // Audio
            Audio,

            // Other
            Scene
        };

        // Add Raw Asset
        UUID add(const char *vertex, const char *fragment, const char *geometry); // Shader
        UUID add(int width, int height, int channels, const unsigned char *data); // Texture
        UUID add(const vec<2> *vertices, size_t v_size, const uint32_t *indices, size_t i_size); // Mesh

        UUID add(int channels, int sample_rate, int samples, short *data); // Audio
        UUID add(const SceneDescriptor &descriptor); // Scene

        // Load Asset
        UUID load(Type type, const std::string &path);
        // TODO: async asset loading
        // TODO: data streams

        bool save(Type type, UUID id, const std::string &path);

        // Unload Asset
        void unload(UUID id);

        // Is Asset ready to be used?
        bool isReady(UUID id);

        // Get Asset Type
        static Type getType(UUID id);

    protected:
        // Resources

        // TODO maybe move into one map

        std::map<UUID, unsigned int> shader{};
        std::map<UUID, unsigned int> texture{};
        std::map<UUID, mesh_t> mesh{};

        std::map<UUID, unsigned int> audio{};

        std::map<UUID, unsigned int> scene{};

        friend class Engine;

        friend class Renderer;

        friend class Mixer;

    private:
        Window &m_window;
        Mixer &m_mixer;

        // TODO temporary/placeholder assets
        // TODO async stuff
        // TODO store path to data for context recovery
        // TODO option to store raw data for android support
    };
}
