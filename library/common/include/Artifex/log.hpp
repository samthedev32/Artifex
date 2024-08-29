/**
 * @brief C++ Logging Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

namespace Log {

// Debug
void d(const char* tag, const char* message, ...);

// Verbose
void v(const char* tag, const char* message, ...);

// Warning
void w(const char* tag, const char* message, ...);

// Error (usually "unrecoverable")
void e(const char* tag, const char* message, ...);

// Assert
void a(int condition, const char* tag, const char* message, ...);

}  // namespace Log