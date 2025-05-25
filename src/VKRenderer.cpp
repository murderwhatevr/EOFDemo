#include "VKRenderer.h"

VKRenderer::VKRenderer() {}

int VKRenderer::Init(GLFWwindow * newWindow) {
    window = newWindow;

    try {
        CreateInstance();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}

void VKRenderer::CreateInstance() {
    // Information for the developer / App info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VK Game";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

    // Information for VKInstance
    VkInstanceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = &appInfo;

    // Create a list to hold instance extensions
    std::vector<const char*> instanceExtensionsList = std::vector<const char*>();

    // macOS-specific setup
    info.flags = 0;
    #ifdef __APPLE__
    info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    // Set up extensions instance will use
    uint32_t glfwExtensionCount = 0; // GLFW may require multiple extensions
    const char** glfwExtensions; // Extensions passed as an array of cstrings, need the pointer (array) to a pointer (cstring)

    // Get GLFW extensions
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Add GLFW extensions to list of extensions
    for (int i = 0; i < glfwExtensionCount; i++) {
        instanceExtensionsList.push_back(glfwExtensions[i]);
    }

    // macOS-specific
    #ifdef __APPLE__
    instanceExtensionsList.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // Required for macOS ( Vulkan )
    instanceExtensionsList.push_back("VK_KHR_get_physical_device_properties2"); // Required for macOS ( Vulkan )
    instanceExtensionsList.push_back("VK_MVK_macos_surface"); // Required for macOS ( Vulkan )
    #endif

    info.enabledExtensionCount = static_cast<uint32_t>(instanceExtensionsList.size());
    info.ppEnabledExtensionNames = instanceExtensionsList.data();

    // TODO: Set up validation layers that instance will use
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = nullptr;

    // Required extensions
    std::cout << "Required extensions:" << std::endl;
    for (const auto& ext : instanceExtensionsList) {
        std::cout << "\t" << ext << std::endl;
    }

    // Create instance
    VkResult result = vkCreateInstance(&info, nullptr, &instance);

    if (result != VK_SUCCESS) {
        std::cerr << "vkCreateInstance failed with code: "<< result << std::endl;
        throw std::runtime_error("Failed to create instance!");
    }
}


VKRenderer::~VKRenderer() {
    if (instance) {
        vkDestroyInstance(instance, nullptr);
    }
}