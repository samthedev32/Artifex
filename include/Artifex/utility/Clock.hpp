#pragma once

struct Clock {
  float past{}, now{};
  float deltaTime{};

  Clock();

  // Update DeltaTime
  void update();

  // Reset Clock
  void reset();

  // TODO
  // ticks
  // timers (sync & async callbacks)

  // Get Current Time (in seconds)
  static float time();
};