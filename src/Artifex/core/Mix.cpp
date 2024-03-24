#include <Artifex/core/Mix.hpp>

#include <stdexcept>

namespace Artifex {

Mix::Mix() {
  // Initialize OpenAL
  if (!((device = alcOpenDevice(nullptr))))
    throw std::runtime_error(std::string(__func__) + ": failed to open device");

  context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);
}

Mix::~Mix() {
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

uuid_t Mix::load(const std::vector<int16_t> &data, int freq) {
  // Audio properties
  ALsizei size = data.size() * sizeof(int16_t); // Size of audio data in bytes
  // ALsizei freq = (float)44100 / (float)2;       // Sample frequency, adjust according to your audio data
  ALenum format = AL_FORMAT_MONO16; // Monophonic 16-bit PCM format

  // Generate buffer and source
  ALuint buffer;
  alGenBuffers(1, &buffer);

  // Load audio data into buffer
  alBufferData(buffer, format, data.data(), size, freq);

  // Check for OpenAL errors
  ALenum alError = alGetError();
  if (alError != AL_NO_ERROR)
    throw std::runtime_error(std::string(__func__) + ": OpenAL error");

  const uuid_t id = uuid_generate();
  buffers[id] = buffer;
  return id;
}

uuid_t Mix::load(const char *path) {
  const std::vector<int16_t> data{};
  int freq{};

  // TODO load audio

  return load(data, freq);
}

void Mix::unload(uuid_t id) {
  alDeleteBuffers(1, &buffers[id]);
  buffers.erase(id);
}

void Mix::play(uuid_t id) {
  // Play the audio
  ALuint source;
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, buffers[id]);
  alSourcePlay(source);

  // Wait for the audio to finish playing
  ALint source_state;
  do {
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  } while (source_state == AL_PLAYING); // && wait);

  // Clean up resources
  alDeleteSources(1, &source);
}

} // namespace Artifex