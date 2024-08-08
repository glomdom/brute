#include "window.hpp"

#include <stdexcept>

Window::Window(int width, int height, const std::string& title)
    : width(width), height(height), title(title) {}

Window::~Window() {
    cleanup();
}

void Window::init() {
    if (!glfwInit()) {
        throw std::runtime_error("failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("failed to create glfw window");
    }
}

void Window::cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void Window::pollEvents() {
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}
