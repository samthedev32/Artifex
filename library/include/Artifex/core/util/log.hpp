/**
 * @brief C++ Logging Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#include <Artifex/log.h>

namespace Log {

// Debug
template<typename... Args>
void d(const char* tag, const char* message, Args... args) {
    ax_debug(tag, message, args...);
}

// Verbose
template<typename... Args>
void v(const char* tag, const char* message, Args... args) {
    ax_verbose(tag, message, args...);
}

// Warning
template<typename... Args>
void w(const char* tag, const char* message, Args... args) {
    ax_warning(tag, message, args...);
}

// Error
template<typename... Args>
void e(const char* tag, const char* message, Args... args) {
    ax_error(tag, message, args...);
}

// Assert
template<typename... Args>
void a(int condition, const char* tag, const char* message, Args... args) {
    ax_assert(tag, message, args...);
}

}  // namespace Log