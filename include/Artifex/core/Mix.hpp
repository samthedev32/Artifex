#pragma once

#include <Artifex/utility/uuid.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Artifex {

// Mixer
struct Mix {
public:
  Mix(const char *devicename = nullptr);
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
  void play(uuid_t id);

  enum DeviceType { STREAM, CAPTURE };

  static std::vector<std::string> getDevices(DeviceType dt = STREAM);

private:
  ALCdevice *device{};
  ALCcontext *context{};

  std::unordered_map<uuid_t, ALuint> buffers{};

  static void checkErrors();
};

} // namespace Artifex