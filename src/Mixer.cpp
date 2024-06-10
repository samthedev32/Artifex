#include <Artifex/Mixer.hpp>

#include <AL/al.h>

#include <stdexcept>

#include <Artifex/Log.hpp>

#define TAG "Mixer"

namespace Artifex {
    Mixer::Mixer(const char *deviceName) {
        openDevice:
        if (!((m_device = alcOpenDevice(deviceName)))) {
            Log::error(TAG, "failed to open device");
            if (deviceName != nullptr) {
                Log::debug(TAG, "attempting to open default device");
                deviceName = nullptr;
                goto openDevice;
            }

            // TODO: unrecoverable error
            return;
        }

        m_context = alcCreateContext(m_device, nullptr);
        use();

        checkErrors();
    }

    Mixer::~Mixer() {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
    }

    unsigned int Mixer::load(int channels, int sample_rate, int samples, short *data) {
        use();

        if (channels < 1 || channels > 2 || sample_rate < 1 || samples < 1 || !data) {
            Log::error(TAG, "Load: invalid resources");
            return nil;
        }

        // Size of audio data in bytes
        const ALsizei size = channels * (int)sizeof(short) * samples;

        // Generate buffer and source
        ALuint id;
        alGenBuffers(1, &id);

        // TODO verify resources

        // Load audio data into buffer
        alBufferData(id, channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, size, sample_rate);

        return id;
    }

    void Mixer::unload(unsigned int id) {
        use();
        alDeleteBuffers(1, &id);
    }

    unsigned int Mixer::create() {
        use();

        unsigned int id;
        alGenSources(1, &id);
        return id;
    }

    void Mixer::destroy(unsigned int source) {
        use();

        alDeleteSources(1, &source);
    }

    void Mixer::set(unsigned int source, unsigned int audio) {
        use();

        alSourcei(source, AL_BUFFER, (int)audio);
    }

    void Mixer::play(unsigned int source) {
        use();

        alSourcePlay(source);
    }

    void Mixer::pause(unsigned int source) {
        use();

        alSourcePause(source);
    }

    void Mixer::stop(unsigned int source) {
        use();

        alSourceStop(source);
    }

    void Mixer::rewind(unsigned int source) {
        use();

        alSourceRewind(source);
    }

    Mixer::State Mixer::state(unsigned int source) {
        use();

        int id;
        alGetSourcei(source, AL_SOURCE_STATE, &id);
        return State(id);
    }

    void Mixer::use() {
        alcMakeContextCurrent(m_context);
    }

    std::vector<std::string> Mixer::getDevices() {
        std::vector<std::string> out;

        if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == AL_FALSE)
            return {};

        const bool support = alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT");
        auto s = alcGetString(nullptr, support ? ALC_ALL_DEVICES_SPECIFIER : ALC_DEVICE_SPECIFIER);

        // retrieve device names
        size_t start = 0, cursor = 0;
        while (start != -1 || s[cursor] != '\0') {
            if (s[cursor] == '\0') {
                out.emplace_back(s + start);
                start = -1;
            } else {
                if (start == -1)
                    start = cursor;
            }
            cursor++;
        }

        return out;
    }

    int Mixer::checkErrors() {
        int count = 0;
        ALenum errorCode;
        while ((errorCode = alGetError()) != AL_NO_ERROR) {
            count++;
            std::string error;
            switch (errorCode) {
                default:
                    error = "UNKNOWN";
                    break;
                case AL_INVALID_ENUM:
                    error = "INVALID_ENUM";
                    break;
                case AL_INVALID_VALUE:
                    error = "INVALID_VALUE";
                    break;
                case AL_INVALID_NAME:
                    error = "INVALID_NAME";
                    break;
                case AL_INVALID_OPERATION:
                    error = "INVALID_OPERATION";
                    break;
                case AL_OUT_OF_MEMORY:
                    error = "OUT_OF_MEMORY";
                    break;
            }
            Log::warning(TAG, "OpenAL Error: %i (%s)", errorCode, error.c_str());
        }

        return count;
    }
}
