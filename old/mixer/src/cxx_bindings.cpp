#include <Artifex/mixer.hpp>
#include <Artifex/mixer.h>

#include <stdexcept>

namespace Artifex {

Mixer::Mixer(const char *deviceName) {
    if (axMixerCreate((axMixer*)&m_mixer, deviceName))
        throw std::runtime_error("failed to create mixer");
}

Mixer::~Mixer() {
    axMixerDestroy((axMixer*)&m_mixer);
}

unsigned int Mixer::load(int channels, int sample_rate, int samples, short *data) {
    return axMixerLoad((axMixer)m_mixer, channels, sample_rate, samples, data);
}

void Mixer::unload(unsigned int id) {
    axMixerUnload((axMixer)m_mixer, id);
}

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

}