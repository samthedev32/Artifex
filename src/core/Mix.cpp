#include <Artifex/core/Mix.hpp>

#include <Artifex/Engine.hpp>

#include <SDL2/SDL_mixer.h>

#include <Artifex/core/Log.hpp>

namespace Artifex {

void Mix::init(Engine *pEngine) {
  if (initialized)
    return;

  engine = pEngine;
  //
  //  if (!Mix_Init(MIX_INIT_MP3))
  //    Log::warning("Mix::init", "An Error Occured during initialization: %s", Mix_GetError());
  //  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)) {
  //    Log::error("Mix::init", "Failed to open Audio Device: %s", Mix_GetError());
  //    return;
  //  }
  //
  //  if (!Mix_AllocateChannels(16))
  //    Log::warning("Mix::init", "Failed to open more channels: %s", Mix_GetError());

  initialized = true;
}

void Mix::deinit() {
  if (!initialized)
    return;

  //  Mix_CloseAudio();

  initialized = false;
}

void Mix::music(uint16_t id, int loops) {
  //  if (Mix_PlayMusic(engine->resource.music[id], loops))
  //    Log::warning("Mix::music", "Failed to play music with id %i", id);
}

void Mix::audio(uint16_t id, int channel, int loops) {
  //  if (Mix_PlayChannel(channel, engine->resource.audio[id], loops))
  //    Log::warning("Mix::audio", "Failed to play audio with id %i", id);
}

} // namespace Artifex