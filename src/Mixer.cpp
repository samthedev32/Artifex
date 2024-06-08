#include <Artifex/Mixer.hpp>
#include <Artifex/AssetManager.hpp>

#include <AL/alc.h>
#include <AL/al.h>

#include <stdexcept>

#include <Artifex/Log.hpp>

#define TAG "Mixer"

namespace Artifex {
    Mixer::Mixer(AssetManager &asset, const char *deviceName) : m_asset(asset) {
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
        use();

        auto audio = m_asset.audio;
        for (auto [key, _]: audio)
            m_asset.unload(key);

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
    }

    ID Mixer::create() {
        use();

        ID id;
        auto &source = m_sources[id];
        alGenSources(1, &source);
        return id;
    }

    void Mixer::destroy(ID source) {
        use();

        alDeleteSources(1, &m_sources[source]);
    }

    void Mixer::set(ID source, ID audio) {
        use();

        alSourcei(m_sources[source], AL_BUFFER, (ALint) m_asset.audio[audio]);
    }

    void Mixer::play(ID source) {
        use();

        alSourcePlay(m_sources[source]);
    }

    void Mixer::pause(ID source) {
        use();

        alSourcePause(m_sources[source]);
    }

    void Mixer::stop(ID source) {
        use();

        alSourceStop(m_sources[source]);
    }

    void Mixer::rewind(ID source) {
        use();

        alSourceRewind(m_sources[source]);
    }

    Mixer::State Mixer::state(ID source) {
        use();

        int out;
        alGetSourcei(m_sources[source], AL_SOURCE_STATE, &out);
        return State(out);
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
        use();

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
