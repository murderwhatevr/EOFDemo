#ifndef VKRENDERER_H
#define VKRENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>

#include "Utilities.h"

class VKRenderer {
public:
    VKRenderer();

    int Init(GLFWwindow * newWindow);

    ~VKRenderer();

private:
    GLFWwindow * window;

    // Validation layers
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Vulkan Components
    VkInstance instance;
    struct {
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
    }mainDevice;
    VkQueue graphicsQueue;

    // Vulkan Functions
    // - Create Functions
    void CreateInstance();
    void CreateLogicalDevice();

    // - Get Functions
    void GetPhysicalDevice();
    void GetLogicalDevice();

    // - Support Functions
    // -- Checker Functions
    bool CheckInstanceExtensionSupport(std::vector<const char*> * checkExtensions);
    bool CheckDeviceSuitable(VkPhysicalDevice device);
    bool CheckValidationLayerSupport();

    // -- Getter Functions
    QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device);
};

#endif //VKRENDERER_H
