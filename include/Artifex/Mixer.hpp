#pragma once

#include <vector>
#include <string>
#include <unordered_map>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace Artifex {
    class AssetManager;

    typedef uint16_t ID;

    class Mixer {
    public:
        Mixer(AssetManager &asset, const char *deviceName = nullptr); // NOLINT(*-explicit-constructor)
        ~Mixer();

        // ---- Source Management

        // Create Source
        ID create();

        // Destroy Source
        void destroy(ID source);

        // Set Audio to Source
        void set(ID source, ID audio);

        // Play
        void play(ID source);

        // Pause
        void pause(ID source);

        // Stop
        void stop(ID source);

        // Rewind
        void rewind(ID source);

        enum State {
            INITIAL = 0x1011,
            PLAYING = 0x1012,
            PAUSED = 0x1013,
            STOPPED = 0x1014
        };

        // Get Source State
        State state(ID source);

        // Use OpenAL Context of the Mixer
        void use();

        static std::vector<std::string> getDevices();

        int checkErrors();

    private:
        AssetManager &m_asset;

        ALCdevice *m_device{};
        ALCcontext *m_context{};

        std::unordered_map<ID, unsigned int> m_sources{};

        // TODO audio input, source pool
    };
}
