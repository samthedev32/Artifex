/**
 * @brief C Audio Capture Library for Artifex
 *
 * @date 2024.06.xx TODO
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Artifex Audio Capture Type
typedef struct axCapture_* axCapture;

struct axCaptureChunk {
    unsigned char* buffer;
    unsigned long samples;
};

// Frame Callback Function
typedef void (*axCaptureCallback)(struct axCaptureChunk* chunk, void* user_ptr);

// Check if capture is valid
int axCaptureIsValid(axCapture capture);

// Create Capture
int axCaptureCreate(axCapture* capture, const char* deviceName);

// Destroy Capture
void axCaptureDestroy(axCapture* capture);

// Start Audio Capture Stream
void axCaptureStart(axCapture capture, axCaptureCallback cb, void* user_ptr);

// Stop Audio Capture Stream
void axCaptureStop(axCapture capture);

// Make OpenAL Context Current
void axCaptureMakeCurrent(axCapture capture);

// Get Audio Devices
int axCaptureGetDevices();

// Check for OpenAL Errors
int axCaptureCheckErrors();

#ifdef __cplusplus
}
#endif