#include "resource_manager.hpp"

#include <stdexcept>

template <typename Resource>
std::shared_ptr<Resource> ResourceManager<Resource>::load(const std::string& filename) {
	auto it = resources.find(filename);
	if (it != resources.end()) {
		return it->second;
	}

	auto resource = std::make_shared<Resource>();
	if (!resource->loadFromFile(filename)) {
		throw std::runtime_error("failed to load resource: " + filename);
	}

	resources[filename] = resource;

	return resource;
}

template <typename Resource>
void ResourceManager<Resource>::unload(const std::string& filename) {
	resources.erase(filename);
}

template <typename Resource>
void ResourceManager<Resource>::unload_all() {
	resources.clear();
}