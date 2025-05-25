#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>

#include "Window.h"
#include "VKRenderer.h"

Window mainWindow;
VKRenderer vkRenderer;

int main() {
    // Initialize window
    if (mainWindow.Init() != 0) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Create Vulkan renderer instance
    if (vkRenderer.Init(mainWindow.GetWindow()) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    //Create the main loop
    mainWindow.MainLoop([] {

    });

    return 0;
}