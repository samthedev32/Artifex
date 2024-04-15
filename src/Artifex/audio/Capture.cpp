#include <Artifex/audio/Capture.hpp>

#include <stdexcept>

namespace Artifex {

Capture::Capture(int bufferSize, unsigned int rate, Format format, const char *deviceName) {
  // Initialize Capture Device
  if (!((device = alcCaptureOpenDevice(deviceName, rate, format, bufferSize))))
    throw std::runtime_error("Capture(): failed to open stream device");

  buffer = new ALbyte[bufferSize];

  checkErrors();
}

Capture::~Capture() {
  stop();
  alcCaptureCloseDevice(device);
}

void Capture::start() { alcCaptureStart(device); }
void Capture::stop() { alcCaptureStop(device); }

void Capture::update() {
  alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
  alcCaptureSamples(device, (ALCvoid *)buffer, sample);
}

std::vector<std::string> Capture::getDevices() {
  std::vector<std::string> out;

  if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == AL_FALSE)
    return {};

  auto s = alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER);

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

void Capture::checkErrors() {
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
    printf("Capture::checkErrors; OpenAL Error: %i (%s)\n", errorCode, error.c_str());
  }
}

} // namespace Artifex