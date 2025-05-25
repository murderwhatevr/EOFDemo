#include "Window.h"

Window::Window() {
    windowWidth = 800;
    windowHeight = 600;
}

Window::Window(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

int Window::Init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Echoes of Faith", nullptr, nullptr);

    if (!mainWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    return 0;
}

void Window::MainLoop(const std::function<void()> &perFrameCallback) {
    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        if (perFrameCallback) {
            perFrameCallback();
        }
    }
}

Window::~Window() {
    if (mainWindow) {
        glfwDestroyWindow(mainWindow);
    }
    glfwTerminate();
}




