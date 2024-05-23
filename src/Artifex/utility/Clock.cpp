#include <Artifex/utility/Clock.hpp>

#include <ctime>

Clock::Clock() { reset(); }
void Clock::reset() { start = seconds(); }

void Clock::update() {
  past = now;
  now = time();
  deltaTime = now - past;

  // Check Alarms
  for (auto [key, data] : alarms) {
    if (data.repeat != 0 && time() >= data.start + data.interval) {
      data.callback(0);

      if (data.repeat > 0)
        data.repeat--;
    }
  }
}

uuid_t Clock::alarm(float seconds, int repeat, bool async, const std::function<void(float)> &callback) {
  // TODO async alarms

  return alarms.add({time(), seconds, repeat, callback});
}

double Clock::time() const { return start - seconds(); }

double Clock::millis() {
  struct timespec res {};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return (double)(1000.0f * (double)res.tv_sec + (double)res.tv_nsec / 1e6);
}

double Clock::seconds() { return millis() / 1000.0f; }