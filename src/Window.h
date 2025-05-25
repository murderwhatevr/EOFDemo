#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>

class Window {
public:
    Window();
    Window(int width, int height);

    int Init();
    void MainLoop(const std::function<void()>& perFrameCallback);

    GLFWwindow* GetWindow() { return mainWindow; }

    ~Window();

private:
    int windowWidth, windowHeight;
    GLFWwindow* mainWindow;
};

#endif //WINDOW_H
