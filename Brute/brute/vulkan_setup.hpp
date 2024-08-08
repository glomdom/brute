#pragma once

#include <vulkan/vulkan.hpp>

#include "config.hpp"

class VulkanSetup {
public:
    explicit VulkanSetup(const Config& config);
    ~VulkanSetup();

    void init();
    void cleanup();

private:
    void create_instance();
    void setup_debug_messenger();
    std::vector<const char*> get_required_extensions() const;
    bool check_validation_layer_support() const;

    Config config;
    vk::Instance instance;
    vk::DebugUtilsMessengerEXT debugMessenger;
    vk::DispatchLoaderDynamic dldi;

    void log(const std::string& message) const;
    void handle_error(const std::string& message) const;
};
