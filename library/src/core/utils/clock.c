#include <Artifex/core/util/clock.h>
#include <stdio.h>
#include <time.h>

unsigned long axClockUNIX() {
    return time(0);
}

// TODO rethink date & time system

int axClockDate(char str[11]) {
    // Get Time
    time_t rawtime;
    struct tm* tm;
    time(&rawtime);
    tm = localtime(&rawtime);

    // Create String
    sprintf(str, "%04d.%02d.%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

    return 0;
}

unsigned long axClockTime(char str[9]) {
    // Get Time
    time_t rawtime;
    struct tm* tm;
    time(&rawtime);
    tm = localtime(&rawtime);

    // Create String
    sprintf(str, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

    return 0;
}

float axClockNow() {
    struct timespec res = {};
    clock_gettime(CLOCK_MONOTONIC, &res);
    float sec = (float)(1000.0f * (float)res.tv_sec + (float)res.tv_nsec / 1e6) / 1000.0f;

    static float start = 0;
    if (start == 0)
        start = sec;
    return sec - start;
}