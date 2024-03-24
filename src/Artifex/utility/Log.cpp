#include <Artifex/utility/Log.hpp>

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace Log {

// Get Current String Time (yyyy.MM.dd HH:mm:ss)
void stime(char *str) {
  // Get Time
  time_t rawtime;
  time(&rawtime);
  const struct tm *timeinfo = localtime(&rawtime);

  // Create String
  sprintf(str, "%04d.%02d.%02d %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
          timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void vcustom(char type, const char *tag, const char *message, va_list args) {
  char msg[1024];
  char date[32];
  stime(date);

  // Prepare for Tag Cutting
  bool cutTag = strlen(tag) > maxTagLen;
  uint8_t tagLen = maxTagLen - (cutTag ? 3 : 0);

  // Create String
  sprintf(msg, "[%19s] %-*.*s%s [%c]  %s\n", date, tagLen, tagLen, tag, cutTag ? "..." : "", type, message);

  // Print Message
  vprintf(msg, args);

  // TODO: Write to File
}

void debug(const char *tag, const char *message, ...) {
#ifndef NDEBUG
  va_list args;
  va_start(args, message);

  vcustom('D', tag, message, args);

  va_end(args);
#endif
}

void verbose(const char *tag, const char *message, ...) {
  va_list args;
  va_start(args, message);

  vcustom('V', tag, message, args);

  va_end(args);
}

void warning(const char *tag, const char *message, ...) {
  va_list args;
  va_start(args, message);

  vcustom('W', tag, message, args);

  va_end(args);
}

void error(const char *tag, const char *message, ...) {
  va_list args;
  va_start(args, message);

  vcustom('E', tag, message, args);

  va_end(args);
}

void assert(const char *tag, const char *message, ...) {
  va_list args;
  va_start(args, message);

  vcustom('A', tag, message, args);

  va_end(args);
}

} // namespace Log