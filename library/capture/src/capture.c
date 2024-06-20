#include <AL/al.h>
#include <AL/alc.h>
#include <Artifex/capture.h>
#include <Artifex/log.h>
#include <malloc.h>

#define TAG "axCapture"

struct axCapture_ {
    ALCdevice* device;
    ALCcontext* context;
};

int axCaptureIsValid(axCapture capture) {
    if (!capture || !capture->device || !capture->context)
        return 0;

    return 1;
}

int axCaptureCreate(axCapture* capture, const char* deviceName) {
    if (!capture) {
        ax_error(TAG, "invalid mixer");
        return 1;
    }

    axCapture cap = *capture = malloc(sizeof(struct axCapture_));
    if (!mix) {
        ax_error(TAG, "failed to allocate memory");
        return 1;
    }

openDevice:
    if (!((mix->playback = alcOpenDevice(deviceName)))) {
        if (deviceName != NULL) {
            ax_warning(TAG, "failed to open device '%s'; attempting to open default", deviceName);
            printf("attempting to open default audio device...\n");
            deviceName = NULL;
            goto openDevice;
        }

        free(*mixer);
        *mixer = NULL;

        ax_error(TAG, "failed to open audio device");
        return 1;
    }

    mix->context = alcCreateContext(mix->playback, NULL);

    if (!mix->context) {
        alcCloseDevice(mix->playback);

        free(*mixer);
        *mixer = NULL;

        ax_error(TAG, "failed to create context");
        return 1;
    }

    ax_verbose(TAG, "initialized mixer");
    return 0;
}

void axMixerDestroy(axMixer* mixer) {
    if (!mixer || !axMixerIsValid(*mixer)) {
        ax_warning(TAG, "invalid mixer");
        return;
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext((*mixer)->context);
    alcCloseDevice((*mixer)->playback);

    free(*mixer);
    *mixer = NULL;

    ax_verbose(TAG, "destroyed mixer");
}

unsigned int axMixerLoad(axMixer mixer, int channels, int sample_rate, int samples, short* data) {
    if (!axMixerIsValid(mixer)) {
        ax_error(TAG, "invalid mixer");
        return 0;
    }

    if (channels < 1 || channels > 2 || sample_rate < 1 || samples < 1 || !data) {
        ax_error(TAG, "invalid resources");
        return 0;
    }

    axMixerMakeCurrent(mixer);

    // Size of audio data in bytes
    const ALsizei size = channels * (int)sizeof(short) * samples;

    // Generate buffer and source
    ALuint audio;
    alGenBuffers(1, &audio);

    // TODO verify resources

    // Load audio data into buffer
    alBufferData(audio, channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, size, sample_rate);

    ax_verbose(TAG, "loaded audio (%i)", audio);
    return audio;
}

void axMixerUnload(axMixer mixer, unsigned int audio) {
    if (!axMixerIsValid(mixer)) {
        ax_warning(TAG, "invalid mixer");
        return;
    }

    axMixerMakeCurrent(mixer);

    alDeleteBuffers(1, &audio);

    ax_verbose(TAG, "unloaded audio (%i)", audio);
}

// void axMixerStart(axMixer mixer, const char* deviceName,  axMixerChunkCallback cb, void* user_ptr) {
//     if (!axMixerIsValid(mixer))
//         return;

//     axMixerMakeCurrent(mixer);

//     if (!((mixer->capture = alcCaptureOpenDevice(deviceName, rate, format, bufferSize))))
//         throw std::runtime_error("Capture(): failed to open stream device");

// alcCaptureStart(device);

//     buffer = new ALbyte[bufferSize];
// }

void axMixerStop(axMixer mixer) {
    alcCaptureStop(device);
    alcCaptureCloseDevice(device);
}

void axMixerMakeCurrent(axMixer mixer) {
    if (!axMixerIsValid(mixer)) {
        ax_warning(TAG, "invalid mixer");
        return;
    }

    alcMakeContextCurrent(mixer->context);
}

int axMixerGetDevices(int isCapture) {
    ax_warning(TAG, "unimplemented");

    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_FALSE)
        return 0;
    return 0;
    // const char support = alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT");
    // const char* s = alcGetString(NULL, isCapture ? ALC_CAPTURE_DEVICE_SPECIFIER : (support ? ALC_ALL_DEVICES_SPECIFIER : ALC_DEVICE_SPECIFIER));

    // // retrieve device names
    // size_t start = 0, cursor = 0;
    // while (start != -1 || s[cursor] != '\0') {
    //     if (s[cursor] == '\0') {
    //         out.emplace_back(s + start);
    //         start = -1;
    //     } else {
    //         if (start == -1)
    //             start = cursor;
    //     }
    //     cursor++;
    // }

    // return out;
}

int axMixerCheckErrors() {
    int count = 0;
    ALenum errorCode;
    while ((errorCode = alGetError()) != AL_NO_ERROR) {
        count++;
        const char* error;
        switch (errorCode) {
            default:
                error = "UNKNOWN";
                break;
            case AL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case AL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case AL_INVALID_NAME:
                error = "INVALID_NAME";
                break;
            case AL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case AL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
        }
        ax_warning(TAG, "%i", errorCode);
    }

    return count;
}