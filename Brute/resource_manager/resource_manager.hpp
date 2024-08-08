#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <format>
#include <shared_mutex>

template <typename Resource>
class ResourceManager {
public:
    [[nodiscard]] std::shared_ptr<Resource> load(const std::string& filename);
    [[nodiscard]] std::shared_ptr<Resource> get(const std::string& filename) const;

    void cleanup();
    void unload(const std::string& filename);
    void unload_all();

private:
    mutable std::shared_mutex resource_mutex;
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
};

template <typename Resource>
[[nodiscard]] std::shared_ptr<Resource> ResourceManager<Resource>::load(const std::string& filename) {
    {
        std::shared_lock lock(resource_mutex);
        
        if (resources.contains(filename)) {
            return resources.at(filename);
        }
    }

    std::unique_lock lock(resource_mutex);
    if (resources.contains(filename)) {
        return resources.at(filename);
    }

    auto resource = std::make_shared<Resource>();
    if (!resource->load_from_file(filename)) {
        throw std::runtime_error(std::format("failed to load resource: {}", filename));
    }

    resources[filename] = resource;

    return resource;
}

template <typename Resource>
[[nodiscard]] std::shared_ptr<Resource> ResourceManager<Resource>::get(const std::string& filename) const {
    std::shared_lock lock(resource_mutex);
    if (resources.contains(filename)) {
        return resources.at(filename);
    }

    throw std::runtime_error(std::format("resource not found: {}", filename));
}

template <typename Resource>
void ResourceManager<Resource>::cleanup() {
    std::unique_lock lock(resource_mutex);
    for (const auto& [filename, resource] : resources) {
        std::cout << std::format("cleaned up resource: {}", filename) << "\n";

        resource->cleanup();
    }

    resources.clear();
}

template <typename Resource>
void ResourceManager<Resource>::unload(const std::string& filename) {
    std::unique_lock lock(resource_mutex);

    resources.erase(filename);
}

template <typename Resource>
void ResourceManager<Resource>::unload_all() {
    std::unique_lock lock(resource_mutex);

    std::cout << "unloading all textures" << "\n";
    resources.clear();
}
