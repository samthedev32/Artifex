#pragma once

#include "ComponentMap.hpp"
#include "uuid.h"

#include <functional>

/**
 * @brief Clock, alarm & timer
 *
 * @warning the clock uses seconds internally
 */
struct Clock {
  // Init Clock
  Clock();

  // Destroy Clock
  ~Clock() = default;

  // Reset Clock
  void reset();

  /**
   * @brief Create Alarm
   *
   * @param seconds Interval of function calls
   * @param repeat Repeating alarm for x times (infinite if negative)
   * @param async Should the alarm be updated asynchronously
   * @param callback Function to call
   */
  uuid_t alarm(float seconds, int repeat, bool async, const std::function<void(float)> &callback);

  /**
   * @brief Update Clock & callbacks
   * @note This function updates DeltaTime
   *
   * @warning for precise sync alarms & accurate deltaTime, call per-frame
   */
  void update();

  /**
   * @brief Get time since clock creation (or reset)
   *
   * @return Time since reset in seconds
   */
  [[nodiscard]] double time() const;

  // Get Current Time (in seconds)
  [[nodiscard]] static double seconds();

  // TODO
  // ticks
  // timers (sync & async callbacks)

public:
  // DeltaTime between last 2 updates
  double deltaTime{};

private:
  double start{}, past{}, now{};

  [[nodiscard]] static double millis();

  struct Alarm {
    double start, interval;
    int repeat;

    std::function<void(float)> callback;
  };

  ComponentMap<Alarm> alarms;
};