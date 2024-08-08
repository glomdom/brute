#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void init();
    void cleanup();
    void pollEvents();
    bool shouldClose() const;

private:
    int width;
    int height;
    std::string title;
    GLFWwindow* window{ nullptr };
};
