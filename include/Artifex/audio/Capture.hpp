#pragma once

#include <Artifex/utility/uuid.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Artifex {

// Basic Audio Recorder
struct Capture {
public:
  enum Format {
    MONO8 = AL_FORMAT_MONO8,
    MONO16 = AL_FORMAT_MONO16,
    STEREO8 = AL_FORMAT_STEREO8,
    STEREO16 = AL_FORMAT_STEREO16
  };

  explicit Capture(int bufferSize = 1024, unsigned int rate = 44100, Format format = MONO16, const char *deviceName = nullptr);
  ~Capture();

  // Start Capture
  void start();

  // Stop Capture
  void stop();

  // Update Buffer
  void update();

  ALbyte *buffer;
  ALint sample;

  static std::vector<std::string> getDevices();

private:
  ALCdevice *device{};

  static void checkErrors();
};

} // namespace Artifex