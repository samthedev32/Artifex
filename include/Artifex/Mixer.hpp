#pragma once

#include <Artifex/types/uuid.hpp>

#include <string>
#include <unordered_map>
#include <vector>

#include <AL/alc.h>

namespace Artifex {
    class Mixer {
    public:
        explicit Mixer(const char *deviceName = nullptr);
        ~Mixer();

        // ---- Resource Management

        // Load Audio Resource
        unsigned int load(int channels, int sample_rate, int samples, short *data);

        // Unload Audio Resource
        void unload(unsigned int id);

        // ---- Source Management

        // Create Source
        unsigned int create();

        // Destroy Source
        void destroy(unsigned int source);

        // Set Audio to Source
        void set(unsigned int source, unsigned int audio);

        // Play
        void play(unsigned int source);

        // Pause
        void pause(unsigned int source);

        // Stop
        void stop(unsigned int source);

        // Rewind
        void rewind(unsigned int source);

        enum State {
            INITIAL = 0x1011,
            PLAYING = 0x1012,
            PAUSED = 0x1013,
            STOPPED = 0x1014
        };

        // Get Source State
        State state(unsigned int source);

        // Use OpenAL Context of the Mixer
        void use();

        // Get Available Devices
        static std::vector<std::string> getDevices();

        // Print OpenAL Errors
        static int checkErrors();

    private:
        ALCdevice *m_device{};
        ALCcontext *m_context{};

        // TODO audio input
    };
}
