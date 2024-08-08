#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <thread>
#include <atomic>
#include <future>
#include <queue>
#include <mutex>
#include <memory>

#include "brute/config.hpp"
#include "resource_manager/resource_manager.hpp"
#include "texture/texture.hpp"
#include "brute/vulkan_setup.hpp"
#include "brute/window.hpp"

class Brute {
public:
    explicit Brute(const Config& config);
    ~Brute();

    std::shared_ptr<Texture> load_texture(const std::string& filename);
    std::future<std::shared_ptr<Texture>> load_texture_async(const std::string& filename);
    void run();

private:
    void process_async_loads();
    void main_loop();
    void cleanup();

    Config config;
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanSetup> vulkanSetup;
    ResourceManager<Texture> textureManager;

    std::atomic<bool> running{ false };
    std::atomic<bool> stopRequested{ false };
    std::thread asyncLoadThread;

    std::queue<std::pair<std::promise<std::shared_ptr<Texture>>, std::string>> asyncLoadQueue;
    std::mutex asyncLoadMutex;
    std::condition_variable asyncLoadCondition;

    void log(const std::string& message) const;
    void handle_error(const std::string& message) const;
};
