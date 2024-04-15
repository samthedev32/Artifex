#pragma once

#include <Artifex/utility/uuid.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Artifex {

// Basic 2D Audio Mixer
struct Mix {
public:
  explicit Mix(const char *deviceName = nullptr);
  ~Mix();

  enum Format {
    MONO8 = AL_FORMAT_MONO8,
    MONO16 = AL_FORMAT_MONO16,
    STEREO8 = AL_FORMAT_STEREO8,
    STEREO16 = AL_FORMAT_STEREO16
  };

  // Load Audio
  uuid_t load(const void *data, int samples, Format format = MONO16, int freq = 44100);

  // UnLoad Audio
  void unload(uuid_t id);

  // Play Audio
  // TODO config, volume, etc
  void play(uuid_t id);

  static std::vector<std::string> getDevices();

private:
  ALCdevice *device{};
  ALCcontext *context{};

  std::unordered_map<uuid_t, ALuint> buffers{};

  static void checkErrors();
};

} // namespace Artifex