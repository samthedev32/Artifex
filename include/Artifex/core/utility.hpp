#pragma once

#include <Artifex/types/types.hpp>

#include <chrono>
#include <fstream>
#include <iostream>

#ifdef __windows
#undef ERROR
#endif

/* Get Current Time */
long time(bool micros = false) {
    if (micros)
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

/* Sleep Function (not recommended) */
void sleep(float seconds) {
    long start = time();

    while (start + seconds * 1000 >= time())
        ;

    return;
}

/* Math Functions */
namespace math {
float decrease(float value, float speed) {
    if (value > 0) {
        if (speed > value)
            value = 0;
        else
            value -= speed;
    } else if (value < 0) {
        if (-speed < value)
            value = 0;
        else
            value += speed;
    }

    return value;
}

/* Slide the output linearly compared to the input with an offset */
float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

/* Clamp an Integer to an interval */
int clamp(int n, int min, int max) {
    if (n < min)
        return min;
    if (n > max)
        return max;
    return n;
}

/* Clamp a Float to an interval */
float clamp(float n, float min, float max) {
    if (n < min)
        return min;
    if (n > max)
        return max;
    return n;
}

/* Clamp a 2D Vector to an interval*/
vec2 clamp(vec2 v, vec2 min, vec2 max) {
    return {clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y)};
}

/* Clamp a 3D Vector to an interval */
vec3 clamp(vec3 v, vec3 min, vec3 max) {
    return {clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y),
            clamp(v.z, min.z, max.z)};
}

/* Detect collision between a point and a rect */
bool collide(vec2 point, vec2 center, vec2 size) {
    float start = center.x - size.x / 2;  // left   /  start
    float end = center.x + size.x / 2;    // right  /  end
    float top = center.y + size.y / 2;    // up     /  top
    float bottom = center.y - size.y / 2; // down   /  bottom

    if (start < point.x && point.x < end)
        if (bottom < point.y && point.y < top)
            return true;
    return false;
}
}; // namespace math

/* String functions */
namespace string {
/* Split a String at another String */
std::string split(std::string s, std::string d, int part) {
    size_t pos_start = 0, pos_end, delim_len = d.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(d, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res[part];
}

/* Split a String at another String and convert it to an Integer */
int isplit(std::string s, std::string d, int part) {
    return stoi(split(s, d, part));
}

/* Split a String at another String and convert it to a Float */
float fsplit(std::string s, std::string d, int part) {
    return stof(split(s, d, part));
}

/* Count a Char's appearances in a String */
int count(std::string s, char d) {
    int c = 0;

    for (int i = 0; i < (int)s.size(); i++)
        if (s[i] == d)
            c++;

    return c;
}

/* Count a String's appearances in a String */
int count(std::string s, std::string d) {
    int c = 0;
    int size[2] = {(int)s.size(), (int)d.size()};

    if (size[0] < size[1])
        return 0;
    if (size[0] == size[1]) {
        if (s == d)
            return 1;
        return 0;
    }

    for (int i = 0; i < size[0] - size[1]; i++) {
        int hits = 0;
        for (int a = 0; a < size[1]; a++) {
            if (s[i + a] == d[a])
                hits++;
        }

        if (hits == size[1])
            c++;
    }
    return c;
}

/* Check if a String appears in a String */
bool isin(std::string s, std::string d) {
    int c = 0;
    int size[2] = {(int)s.size(), (int)d.size()};

    if (size[0] < size[1])
        return false;

    if (size[0] == size[1]) {
        if (s == d)
            return true;
        return false;
    }

    for (int i = 0; i < size[0] - size[1]; i++) {
        int hits = 0;
        for (int a = 0; a < size[1]; a++) {
            if (s[i + a] == d[a])
                hits++;
        }

        if (hits == size[1])
            return true;
    }
    return false;
}

/* Lowercase a String */
std::string lowercase(std::string s) {
    int difference = 'a' - 'A';
    std::string ss = s;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            ss[i] += difference;
        }
    }

    return ss;
}

/* Remove Multiple Spaces From String */
std::string multispace(std::string s) {
    std::string ss;
    bool ms = false;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == ' ') {
            if (!ms)
                ss += s[i];
            ms = true;
        } else {
            ss += s[i];
            ms = false;
        }
    }
    return ss;
}

/* Remove spaces from a String */
std::string despace(std::string s) {
    std::string ss;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t' && s[i] != '\0')
            ss += s[i];
    }
    return ss;
}

/* Convert an Integer into a String */
std::string itos(int num) { return std::to_string(num); }

/* Convert a Float to a String */
std::string ftos(float num) { return std::to_string(num); }

/* Convert a Char to a String */
std::string ctos(char c) { return std::string("") + c; }
}; // namespace string

/* Basic Logging Functions */
namespace LOG {
// The Breakpoint's number for LOG::DEBUG()
int BREAKPOINT = 0;

/* Log an Error */
void ERROR(std::string function, std::string message, int code = -1) {
    fprintf(stderr, "ERROR: %s; %s\n", function.c_str(), message.c_str());
    exit(code);
}

/* Log an Error */
void ERROR(std::string message, int code = -1) {
    fprintf(stderr, "ERROR: %s\n", message.c_str());
    exit(code);
}

/* Log a Warning */
void WARNING(std::string function, std::string message) {
    fprintf(stdout, "WARNING: %s; %s\n", function.c_str(), message.c_str());
}

/* Log a Warning */
void WARNING(std::string message) {
    fprintf(stdout, "WARNING: %s\n", message.c_str());
}

/* Log a Debug Message */
void DEBUG(std::string function, std::string message) {
    fprintf(stdout, "DEBUG: %s; %s\n", function.c_str(), message.c_str());
}

/* Log a Debug Message */
static void DEBUG(std::string message) {
    fprintf(stdout, "DEBUG: %s\n", message.c_str());
}

static void DEBUG() {
    fprintf(stdout, "DEBUG: BREAKPOINT #%i\n", BREAKPOINT);
    BREAKPOINT++;
}
} // namespace LOG

/* Flip a surface */
void SDL_FlipSurface(SDL_Surface *surface) {
    SDL_LockSurface(surface);

    int pitch = surface->pitch;   // row size
    char *temp = new char[pitch]; // intermediate buffer
    char *pixels = (char *)surface->pixels;

    for (int i = 0; i < surface->h / 2; ++i) {
        // get pointers to the two rows to swap
        char *row1 = pixels + i * pitch;
        char *row2 = pixels + (surface->h - i - 1) * pitch;

        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }

    delete[] temp;

    SDL_UnlockSurface(surface);
}