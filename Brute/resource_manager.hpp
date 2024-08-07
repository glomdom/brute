#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

template <typename Resource>
class ResourceManager {
public:
    std::shared_ptr<Resource> load(const std::string& filename);
    std::shared_ptr<Resource> get(const std::string& filename) const;

    void cleanup();
    void unload(const std::string& filename);
    void unload_all();

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
};

template <typename Resource>
std::shared_ptr<Resource> ResourceManager<Resource>::load(const std::string& filename) {
    auto it = resources.find(filename);
    if (it != resources.end()) {
        return it->second;
    }

    auto resource = std::make_shared<Resource>();
    if (!resource->load_from_file(filename)) {
        throw std::runtime_error("failed to load resource: " + filename);
    }

    resources[filename] = resource;

    return resource;
}

template <typename Resource>
std::shared_ptr<Resource> ResourceManager<Resource>::get(const std::string& filename) const {
    auto it = resources.find(filename);
    if (it != resources.end()) {
        return it->second;
    }

    throw std::runtime_error("resource not found: " + filename);
}

template <typename Resource>
void ResourceManager<Resource>::cleanup() {
    for (auto& resourcePair : resources) {
        std::cout << "cleaned up texture: " << resourcePair.first.c_str() << "\n";

        resourcePair.second->cleanup();
    }

    this->unload_all();
}

template <typename Resource>
void ResourceManager<Resource>::unload(const std::string& filename) {
    resources.erase(filename);
}

template <typename Resource>
void ResourceManager<Resource>::unload_all() {
    std::cout << "unloading all textures" << "\n";

    resources.clear();
}
