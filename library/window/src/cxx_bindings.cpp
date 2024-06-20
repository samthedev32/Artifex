#include <Artifex/window.h>

#include <Artifex/window.hpp>
#include <stdexcept>

namespace Artifex {

Window::Window(const std::string& title, int width, int height) {
    if (axWindowCreate((axWindow*)&m_window, title.c_str(), width, height))
        throw std::runtime_error("failed to create window");
}

Window::~Window() {
    axWindowDestroy((axWindow*)&m_window);
}

bool Window::update() {
    return axWindowUpdate((axWindow)m_window);
}

void Window::exit() {
    axWindowExit((axWindow)m_window);
}

void Window::getSize(int& width, int& height) {
    axWindowSize((axWindow)m_window, &width, &height);
}

float Window::getRatio() {
    return axWindowRatio((axWindow)m_window);
}

void Window::setVSync(int interval) {
    axWindowSetVSync((axWindow)m_window, interval);
}

int Window::getCursor(float& x, float& y) {
    return axWindowGetCursor((axWindow)m_window, &x, &y);
}

bool Window::getKeyState(axWindowKey key) {
    return axWindowGetKeyState((axWindow)m_window, key);
}

Window::procAddr Window::getProcAddress(const char* procname) {
    return axWindowGetAddress(procname);
}

void Window::makeCurrent() {
    axWindowMakeCurrent((axWindow)m_window);
}

}  // namespace Artifex