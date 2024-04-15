#include <Artifex/utility/Clock.hpp>

#include <ctime>

Clock::Clock() { reset(); }

void Clock::update() {
  past = now;
  now = time();
  deltaTime = now - past;
}

void Clock::reset() {
  past = now = time();
  deltaTime = 0;
}

float Clock::time() {
  struct timespec res {};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return (float)(1000.0f * (float)res.tv_sec + (float)res.tv_nsec / 1e6) / 1000.0f;
}