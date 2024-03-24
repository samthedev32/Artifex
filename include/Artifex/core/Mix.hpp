#pragma once

#include <Artifex/utility/uuid.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace Artifex {

// Mixer
struct Mix {
public:
  Mix();
  ~Mix();

  // Load Audio
  uuid_t load(const std::vector<int16_t> &data, int freq);
  uuid_t load(const char *path);

  // UnLoad Audio
  void unload(uuid_t id);

  // Play Audio
  void play(uuid_t id);

private:
  ALCdevice *device{};
  ALCcontext *context{};

  std::unordered_map<uuid_t, ALuint> buffers{};
};

} // namespace Artifex