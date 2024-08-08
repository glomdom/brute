#include "brute.hpp"

#include <iostream>
#include <format>
#include <stdexcept>

Brute::Brute(const Config& config) : config(config) {
    window = std::make_unique<Window>(config.windowWidth, config.windowHeight, config.windowTitle);
    vulkanSetup = std::make_unique<VulkanSetup>(config);
}

Brute::~Brute() {
    if (running) {
        stopRequested = true;

        if (asyncLoadThread.joinable()) {
            asyncLoadCondition.notify_all();
            asyncLoadThread.join();
        }
    }

    cleanup();
}

std::shared_ptr<Texture> Brute::load_texture(const std::string& filename) {
    log(std::format("loading texture: {}", filename));
    auto texture = textureManager.load(filename);

    if (texture) {
        log(std::format("loaded texture: {}", filename));
    } else {
        handle_error(std::format("failed to load texture: {}", filename));
    }

    return texture;
}

std::future<std::shared_ptr<Texture>> Brute::load_texture_async(const std::string& filename) {
    std::promise<std::shared_ptr<Texture>> promise;
    std::future<std::shared_ptr<Texture>> future = promise.get_future();

    {
        std::lock_guard<std::mutex> lock(asyncLoadMutex);
        asyncLoadQueue.emplace(std::move(promise), filename);
    }

    asyncLoadCondition.notify_one();

    return future;
}

void Brute::run() {
    if (running) {
        throw std::runtime_error("engine is already running");
    }

    running = true;
    stopRequested = false;

    window->init();
    vulkanSetup->init();

    asyncLoadThread = std::thread(&Brute::process_async_loads, this);

    main_loop();
}

void Brute::main_loop() {
    while (!window->shouldClose() && !stopRequested) {
        window->pollEvents();
    }

    running = false;
    stopRequested = true;

    if (asyncLoadThread.joinable()) {
        asyncLoadCondition.notify_all();
        asyncLoadThread.join();
    }

    cleanup();
}

void Brute::process_async_loads() {
    while (!stopRequested) {
        std::unique_lock<std::mutex> lock(asyncLoadMutex);
        asyncLoadCondition.wait(lock, [this] { return !asyncLoadQueue.empty() || stopRequested; });

        while (!asyncLoadQueue.empty()) {
            auto [promise, filename] = std::move(asyncLoadQueue.front());
            asyncLoadQueue.pop();

            lock.unlock();

            try {
                auto texture = load_texture(filename);
                promise.set_value(texture);
            } catch (...) {
                promise.set_exception(std::current_exception());
            }

            lock.lock();
        }
    }
}

void Brute::cleanup() {
    if (vulkanSetup) {
        vulkanSetup->cleanup();
    }
    
    if (window) {
        window->cleanup();
    }

    textureManager.cleanup();
}
void Brute::log(const std::string& message) const {
    std::cout << message << std::endl;
}

void Brute::handle_error(const std::string& message) const {
    std::cerr << message << std::endl;
}
