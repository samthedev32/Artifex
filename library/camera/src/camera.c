#include <Artifex/camera.h>
#include <Artifex/log.h>
#include <libuvc/libuvc.h>
#include <stdlib.h>

#include "yuyv.h"

#define TAG "axCamera"

struct axCamera_ {
    uvc_context_t* ctx;
    uvc_device_t* dev;
    uvc_stream_ctrl_t ctrl;
    uvc_device_handle_t* devh;
    struct libusb_device_handle* usb_devh;

    axCameraFrameCallback cb;
    void* user;
};

void callback(struct uvc_frame* fframe, void* user_ptr);

int axCameraIsValid(axCamera camera) {
    if (!camera || !camera->ctx || !camera->dev)
        return 0;

    return 1;
}

int axCameraCreate(axCamera* camera, int width, int height, int fps) {
    if (!camera) {
        ax_error(TAG, "invalid camera");
        return 1;
    }

    axCamera cam = *camera = malloc(sizeof(struct axCamera_));
    if (!cam) {
        ax_error(TAG, "failed to allocate memory");
        return 1;
    }

    cam->user = NULL;

    if (uvc_init(&cam->ctx, NULL) < UVC_SUCCESS) {
        free(*camera);
        *camera = NULL;

        ax_error(TAG, "failed to initialize libuvc");
        return 1;
    }

    // res = uvc_find_device(ctx, &dev, 0x2833, 0x0211, NULL);
    if (uvc_find_device(cam->ctx, &cam->dev, 0, 0, NULL) < UVC_SUCCESS) {
        uvc_exit(cam->ctx);

        free(*camera);
        *camera = NULL;

        ax_error(TAG, "failed to find camera");
        return 1;
    }

    if (uvc_open(cam->dev, &cam->devh) < UVC_SUCCESS) {
        uvc_exit(cam->ctx);

        free(*camera);
        *camera = NULL;

        ax_error(TAG, "failed to open camera");
        return 1;
    }

    // cam->usb_devh = uvc_get_libusb_handle(cam->devh);

    if (uvc_get_stream_ctrl_format_size(cam->devh, &cam->ctrl, UVC_FRAME_FORMAT_YUYV,
                                        width, height, fps) < UVC_SUCCESS) {
        uvc_close(cam->devh);
        uvc_exit(cam->ctx);

        free(*camera);
        *camera = NULL;

        ax_error(TAG, "failed to get format/size");
        return 1;
    }

    // uvc_print_diag(c->devh, stdout);

    ax_verbose(TAG, "initialized camera");
    return 0;
}

void axCameraDestroy(axCamera* camera) {
    if (!axCameraIsValid(*camera)) {
        ax_warning(TAG, "invalid camera");
        return;
    }

    uvc_stop_streaming((*camera)->devh);
    uvc_close((*camera)->devh);
    uvc_exit((*camera)->ctx);

    free(*camera);
    *camera = NULL;

    ax_verbose(TAG, "destroyed camera");
}

int axCameraStart(axCamera camera, axCameraFrameCallback cb, void* user_ptr) {
    if (!axCameraIsValid(camera)) {
        ax_error(TAG, "invalid camera");
        return 1;
    }

    if (camera->user) {
        ax_warning(TAG, "already streaming");
        return 1;
    }

    camera->cb = cb;
    camera->user = user_ptr;

    if (uvc_start_streaming(camera->devh, &camera->ctrl, callback, camera, 0) <
        UVC_SUCCESS) {
        camera->cb = NULL;
        camera->user = NULL;

        ax_warning(TAG, "failed to start streaming");
        return 1;
    }

    ax_verbose(TAG, "started streaming");
    return 0;
}

void axCameraStop(axCamera camera) {
    if (!axCameraIsValid(camera)) {
        ax_warning(TAG, "invalid camera");
        return;
    }

    uvc_stop_streaming(camera->devh);
    camera->user = NULL;

    ax_verbose(TAG, "stopped streaming");
}

// Callback Function
void callback(struct uvc_frame* fframe, void* user_ptr) {
    if (!fframe || !user_ptr)
        return;

    if (fframe->frame_format != UVC_FRAME_FORMAT_YUYV)
        return;

    axCamera camera = user_ptr;

    // Prepare RGB Frame
    struct axCameraFrame frame;
    frame.width = fframe->width;
    frame.height = fframe->height;
    frame.data = malloc(fframe->width * fframe->height * 3);

    if (!frame.data)
        return;

    // Convert YUYV to RGB
    yuyv_to_rgb(fframe->data, frame.data, fframe->width, fframe->height);

    // Call Function
    camera->cb(&frame, camera->user);

    // Free Frame Data
    free(frame.data);
}
