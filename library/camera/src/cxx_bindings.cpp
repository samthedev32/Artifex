#include <Artifex/camera.h>

#include <Artifex/camera.hpp>
#include <stdexcept>

namespace Artifex {

void cb(struct axCameraFrame* frame, void* user_prt);

Camera::Camera(int width, int height, int fps) {
    if (axCameraCreate((axCamera*)&m_camera, width, height, fps))
        throw std::runtime_error("failed to create camera");
}

Camera::~Camera() {
    axCameraDestroy(&m_camera);
}

int Camera::start(std::function<void(const Frame&)> callback) {
    return axCameraStart(m_camera, cb, &callback);
}

void Camera::stop() {
    axCameraStop(m_camera);
}

void cb(struct axCameraFrame* frame, void* user_prt) {
    auto func = (std::function<void(const Camera::Frame&)>*)user_prt;

    Camera::Frame f;
    f.data = frame->data;
    f.width = frame->width, f.height = frame->height;

    (*func)(f);
}

}  // namespace Artifex