// A Basic Lightweight Music Player/Mixer for Artifex
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <map>

struct audio
{
    void *data = NULL;
    int type = -1;

    int ch = -1; // The track the EFFECT is played on
};

class Mixer
{
private:
    std::map<std::string, audio> track;

public:
    Mixer(int channels = 8);

    bool effect(std::string path, std::string name = "effect");
    bool music(std::string path, std::string name = "music");

    bool play(std::string name, int loop = 0);

    bool pause(std::string name);
    bool resume(std::string name);
};

// Init Mixer
Mixer::Mixer(int channels)
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, channels, 2048);
}

// Load A Sound Effect
bool Mixer::effect(std::string path, std::string name)
{
    if (track[name].data != NULL)
        return false;

    track[name].data = (void *)Mix_LoadWAV(path.c_str());
    track[name].type = 0; // Set Type to Effect

    return true;
}

// Load A Music
bool Mixer::music(std::string path, std::string name)
{
    if (track[name].data != NULL)
        return false;

    track[name].data = (void *)Mix_LoadMUS(path.c_str());
    track[name].type = 1; // Set Type to Music

    return true;
}

bool Mixer::play(std::string name, int loop)
{
    switch (track[name].type)
    {
    case 0:
        //  Effect
        track[name].ch = Mix_PlayChannel(-1, (Mix_Chunk *)track[name].data, loop);
        break;

    case 1:
        // Music
        Mix_PlayMusic((Mix_Music *)track[name].data, loop);
        break;

    default:
        // Unknown or No Audio
        return false;
        break;
    }

    return true;
}

bool Mixer::pause(std::string name)
{
    switch (track[name].type)
    {
    case 0:
        //  Effect
        Mix_Pause(track[name].ch);
        break;

    case 1:
        // Music
        Mix_PauseMusic();
        break;

    default:
        // Unknown or No Audio
        return false;
        break;
    }

    return true;
}

bool Mixer::resume(std::string name)
{
    switch (track[name].type)
    {
    case 0:
        //  Effect
        Mix_Resume(track[name].ch);
        break;

    case 1:
        // Music
        Mix_ResumeMusic();
        break;

    default:
        // Unknown or No Audio
        return false;
        break;
    }

    return true;
}

// TODO: Set Position
// Mix_SetPosition(channel, angle, distance * strength);