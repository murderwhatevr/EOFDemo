#include "VKRenderer.h"

VKRenderer::VKRenderer() {}

int VKRenderer::Init(GLFWwindow * newWindow) {
    window = newWindow;

    try {
        CreateInstance();
        GetPhysicalDevice();
        CreateLogicalDevice();
    }
    catch (std::runtime_error &e) {
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
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 4, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 4, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

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

    // Check if the extensions are supported
    if (!CheckInstanceExtensionSupport(&instanceExtensionsList)) {
        throw std::runtime_error("Vulkan does not support the required extensions!");
    }

    // Create instance
    VkResult result = vkCreateInstance(&info, nullptr, &instance);

    if (result != VK_SUCCESS) {
        std::cerr << "vkCreateInstance failed with code: "<< result << "\n";
        throw std::runtime_error("Failed to create instance!");
    }
}

void VKRenderer::GetPhysicalDevice() {
    // Enumerate physical devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // Check if there are any devices
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    // Get the list of physical devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Get the first suitable device
    for (const auto &device : devices) {
        if (CheckDeviceSuitable(device)) {
            mainDevice.physicalDevice = device;
            break;
        }
    }
}

void VKRenderer::CreateLogicalDevice() {
    // Get the queue family indices
    QueueFamilyIndices indices = GetQueueFamilyIndices(mainDevice.physicalDevice);

    // Queue the logical device needs
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily; // The index of the queue family
    queueCreateInfo.queueCount = 1; // The number of queues in the queue family
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority; // Handling of multiple queues

    // Information for the logical device
    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1; // The number of queueCreateInfos
    deviceInfo.pQueueCreateInfos = &queueCreateInfo; // List of queueCreateInfos
    deviceInfo.enabledExtensionCount = 0; // The number of extensions
    deviceInfo.ppEnabledExtensionNames = nullptr; // List of extensions

    // Physical device features that the logical device will use
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceInfo.pEnabledFeatures = &deviceFeatures; // Physical device features

    // Create the logical device
    VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceInfo, nullptr, &mainDevice.logicalDevice);
    if (result != VK_SUCCESS) {
        std::cerr << "vkCreateDevice failed with code: "<< result << "\n";
        throw std::runtime_error("Failed to create logical device!");
    }

    // Get the logical device's queues
    vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
}

bool VKRenderer::CheckInstanceExtensionSupport(std::vector<const char*> *checkExtensions) {
    // Get the number of extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    // Create a list of vkExtensionProperties
    std::vector<VkExtensionProperties> vulkanExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, vulkanExtensions.data());

    // Check if the extensions are supported
    for (const auto &checkExtension : *checkExtensions) {
        bool hasExtension = false;
        for (const auto &extension : vulkanExtensions) {
            if (strcmp(checkExtension, extension.extensionName) == 0) {
                hasExtension = true;
                break;
            }
        }
        if (!hasExtension) {
            return false;
        }
    }
    return true;
}

bool VKRenderer::CheckDeviceSuitable(VkPhysicalDevice device) {
    /*
    // Information about the device
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    // Information about the device's features'
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    */

    QueueFamilyIndices indices = GetQueueFamilyIndices(device);

    return indices.isValid();
}

QueueFamilyIndices VKRenderer::GetQueueFamilyIndices(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    // Get the number of queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Go thru the queue families
    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        // Check if the queue family has at least one queue
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i; // If the queue family is valid, get the index
        }
        // Check if the queue families are valid and stop searching if so
        if (indices.isValid()) {
            break;
        }

        i++;
    }
    return indices;
}


VKRenderer::~VKRenderer() {
    // Destroy the logical device
    if (mainDevice.logicalDevice) {
        vkDestroyDevice(mainDevice.logicalDevice, nullptr);
        mainDevice.logicalDevice = VK_NULL_HANDLE;
    }

    // Destroy the instance
    if (instance) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}