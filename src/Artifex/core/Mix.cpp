#include <Artifex/core/Mix.hpp>

#include <stdexcept>

namespace Artifex {

Mix::Mix(const char *devicename) {
  // Initialize OpenAL
  if (!((device = alcOpenDevice(devicename))))
    throw std::runtime_error(std::string(__func__) + ": failed to open device");

  context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);

  checkErrors();
}

Mix::~Mix() {
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

uuid_t Mix::load(const void *data, int samples, Format format, int freq) {
  // Calculate Audio Size
  const bool is8bit = format == MONO8 || format == STEREO8;
  const bool isStereo = format == STEREO8 || format == STEREO16;

  const ALsizei dataSize = is8bit ? sizeof(int8_t) : sizeof(int16_t);
  const ALsizei size = (isStereo ? 2 : 1) * dataSize * samples; // Size of audio data in bytes

  // Generate buffer and source
  ALuint buffer;
  alGenBuffers(1, &buffer);

  // Load audio data into buffer
  alBufferData(buffer, format, data, size, freq);

  checkErrors();

  const uuid_t id = uuid_generate();
  buffers[id] = buffer;
  return id;
}

void Mix::unload(uuid_t id) {
  alDeleteBuffers(1, &buffers[id]);
  buffers.erase(id);
}

void Mix::play(uuid_t id) {
  // Play the audio
  ALuint source;
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, static_cast<ALint>(buffers[id]));
  alSourcePlay(source);

  // Wait for the audio to finish playing
  ALint source_state;
  do {
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  } while (source_state == AL_PLAYING); // && wait);

  // Clean up resources
  alDeleteSources(1, &source);
}

std::vector<std::string> Mix::getDevices(DeviceType dt) {
  std::vector<std::string> out;

  if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == AL_FALSE)
    return {};

  const bool support = alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT");
  const auto stream = support ? ALC_ALL_DEVICES_SPECIFIER : ALC_DEVICE_SPECIFIER;
  auto s = alcGetString(nullptr, dt == STREAM ? stream : ALC_CAPTURE_DEVICE_SPECIFIER);

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

void Mix::checkErrors() {
  ALenum errorCode;
  while ((errorCode = alGetError()) != AL_NO_ERROR) {
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
    printf("Mix::checkErrors; OpenAL Error: %i (%s)\n", errorCode, error.c_str());
  }
}

} // namespace Artifex