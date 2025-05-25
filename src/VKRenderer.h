#ifndef VKRENDERER_H
#define VKRENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

class VKRenderer {
public:
    VKRenderer();

    int Init(GLFWwindow * newWindow);

    ~VKRenderer();

private:
    GLFWwindow * window;

    // Vulkan Components
    VkInstance instance;

    // Vulkan Functions
    void CreateInstance();
};

#endif //VKRENDERER_H
