#include <Artifex/core/util/clock.h>
#include <Artifex/core/util/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Colors
#define AX_LOG_COLOR_RED 31
#define AX_LOG_COLOR_GREEN 32
#define AX_LOG_COLOR_YELLOW 33
#define AX_LOG_COLOR_BLUE 34
#define AX_LOG_COLOR_MAGENTA 35
#define AX_LOG_COLOR_CYAN 36
#define AX_LOG_COLOR_WHITE 37

// Cut-off Length of Tag
#define AX_LOG_TAG_LEN 16

// Enable/Disable Logs of the same severity with the same tag
#define AX_LOG_GROUPING 1

// Log Grouping Timeout (in seconds)
#define AX_LOG_GROUPING_TIMEOUT 30

// Get Current String Time (yyyy.MM.dd HH:mm:ss)
void stime(char* str);

// Log Custom
void ax__custom(char type, int color, const char* tag, const char* message, va_list args);

void ax_debug(const char* tag, const char* message, ...) {
#ifndef NDEBUG
    va_list args;
    va_start(args, message);

    ax__custom('D', AX_LOG_COLOR_GREEN, tag, message, args);

    va_end(args);
#endif
}

void ax_verbose(const char* tag, const char* message, ...) {
    va_list args;
    va_start(args, message);

    ax__custom('V', AX_LOG_COLOR_BLUE, tag, message, args);

    va_end(args);
}

void ax_warning(const char* tag, const char* message, ...) {
    va_list args;
    va_start(args, message);

    ax__custom('W', AX_LOG_COLOR_YELLOW, tag, message, args);

    va_end(args);
}

void ax_error(const char* tag, const char* message, ...) {
    va_list args;
    va_start(args, message);

    ax__custom('E', AX_LOG_COLOR_RED, tag, message, args);

    va_end(args);
}

void ax_assert(int condition, const char* tag, const char* message, ...) {
    if (!condition) {
        va_list args;
        va_start(args, message);

        ax__custom('A', AX_LOG_COLOR_MAGENTA, tag, message, args);

        va_end(args);
        exit(1);
    }
}

void stime(char* str) {
    axClockDate(str);
    str[11] = ' ';
    axClockTime(str + 12);
    // str[20] = 0;
}

char axLog__prevTag[AX_LOG_TAG_LEN];
char axLog__prevType;
unsigned long axLog__prevTime;

void ax__custom(char type, int color, const char* tag, const char* message, va_list args) {
    int same = AX_LOG_GROUPING && time(0) < axLog__prevTime + AX_LOG_GROUPING_TIMEOUT && (axLog__prevType == type && !strncmp(tag, axLog__prevTag, AX_LOG_TAG_LEN));

    strncpy(axLog__prevTag, tag, AX_LOG_TAG_LEN);
    axLog__prevType = type;
    axLog__prevTime = time(0);

    char msg[1024];
    char date[11], time[9];
    axClockDate(date);
    axClockTime(time);

    // Prepare for Tag Cutting
    int cutTag = strlen(tag) > AX_LOG_TAG_LEN;
    int tagLen = AX_LOG_TAG_LEN - (cutTag ? 3 : 0);

    // Create String
    if (same) {
        // Print Message (spaces + timestamp + tag)
        sprintf(msg, "%*s\n", (int)(4 + 21 + AX_LOG_TAG_LEN + 3 + strlen(message)), message);
    } else {
        // %-*.*s%s to start from left
        sprintf(msg, "[%10s %8s] %-*.*s%s [\e[1m\e[%im%c\e[0m]  %s\n", date, time, tagLen, tagLen, tag,
                cutTag ? "..." : "", color, type, message);
    }

    // Print Message
    vprintf(msg, args);

    // TODO: Write to File
}