#pragma once

#include <Artifex/types/uuid.hpp>

#include <string>
#include <unordered_map>
#include <vector>

struct ALCdevice;
struct ALCcontext;

namespace Artifex {
    class AssetManager;

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

        struct ALCdevice *m_device{};
        struct ALCcontext *m_context{};

        std::unordered_map<ID, unsigned int> m_sources{};

        // TODO audio input, source pool
    };
}
