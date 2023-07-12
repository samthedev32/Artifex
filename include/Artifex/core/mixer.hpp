// Music Player/Mixer for Artifex
#pragma once

#include <artifex/core/load.hpp>

class Mixer {
  private:
  public:
    std::map<std::string, audio> track;

    Mixer();

    bool play(std::string name, int loop = 0);

    void pause(std::string name);
    void resume(std::string name);

    bool position(std::string name, int angle, int distance);
};

// Init Mixer
Mixer::Mixer() {
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(16);
}

bool Mixer::play(std::string name, int loop) {

    track[name].ch = Mix_PlayChannel(-1, (Mix_Chunk *)track[name].source, loop);

    return track[name].ch != -1;
}

void Mixer::pause(std::string name) { Mix_Pause(track[name].ch); }

void Mixer::resume(std::string name) { Mix_Resume(track[name].ch); }

bool Mixer::position(std::string name, int angle, int distance) {
    return Mix_SetPosition(track[name].ch, angle, distance) != 0;
}

// TODO: Set Position
// Mix_SetPosition(channel, angle, distance * strength);