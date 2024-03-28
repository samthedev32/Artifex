#include <Artifex/core/Mix.hpp>

#include "stb_vorbis.c"

using namespace Artifex;

int main() {
  auto devices = Mix::getDevices();
  int d = -1;
  if (devices.size() > 1) {
    printf("Select Device:\n");
    for (int i = 0; i < devices.size(); i++) {
      printf("[%i]: %s\n", i, devices[i].c_str());
    }
    scanf("%i", &d);
  }

  Mix mix(d == -1 ? nullptr : devices[d].c_str());

  int channels, sample_rate, samples;
  short *data; // stb_vorbis loads 16 bit audio

  // Load Audio 1
  samples = stb_vorbis_decode_filename("../../../examples/mix_simple/audio/game.ogg", &channels, &sample_rate, &data);
  uuid_t id1 = mix.load(data, samples, channels == 2 ? Mix::STEREO16 : Mix::MONO16, sample_rate);
  free(data);

  // Load Audio 2
  samples = stb_vorbis_decode_filename("../../../examples/mix_simple/audio/music.ogg", &channels, &sample_rate, &data);
  uuid_t id2 = mix.load(data, samples, channels == 2 ? Mix::STEREO16 : Mix::MONO16, sample_rate);
  free(data);

  mix.play(id1);
  mix.play(id2);

  return 0;
}