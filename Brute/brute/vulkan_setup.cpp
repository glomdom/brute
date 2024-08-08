#include "vulkan_setup.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

VulkanSetup::VulkanSetup(const Config& config) : config(config), instance(nullptr), debugMessenger(nullptr) {}

VulkanSetup::~VulkanSetup() {
    cleanup();
}

void VulkanSetup::init() {
    create_instance();
    if (config.enableValidationLayers) {
        setup_debug_messenger();
    }
}

void VulkanSetup::create_instance() {
    if (config.enableValidationLayers && !check_validation_layer_support()) {
        throw std::runtime_error("Validation layers requested, but aren't available");
    }

    vk::ApplicationInfo appInfo{};
    appInfo.pApplicationName = config.windowTitle.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Brute";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    vk::InstanceCreateInfo createInfo{};
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = get_required_extensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (config.enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(config.validationLayers.size());
        createInfo.ppEnabledLayerNames = config.validationLayers.data();

        debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT{};
        createInfo.pNext = &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    instance = vk::createInstance(createInfo);
    dldi.init(instance, vkGetInstanceProcAddr);
}

void VulkanSetup::setup_debug_messenger() {
    if (!config.enableValidationLayers) {
        return;
    }

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    createInfo.pfnUserCallback = [](
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
        ) -> VkBool32 {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        };

    debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
}

std::vector<const char*> VulkanSetup::get_required_extensions() const {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (config.enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool VulkanSetup::check_validation_layer_support() const {
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char* layerName : config.validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void VulkanSetup::cleanup() {
    if (config.enableValidationLayers && debugMessenger) {
        if (instance) {
            instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
            debugMessenger = nullptr;
        }
    }

    if (instance) {
        instance.destroy();
        instance = nullptr;
    }
}

void VulkanSetup::log(const std::string& message) const {
    std::cout << message << std::endl;
}

void VulkanSetup::handle_error(const std::string& message) const {
    std::cerr << message << std::endl;
}
