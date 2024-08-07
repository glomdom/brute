#pragma once

#include <unordered_map>
#include <memory>
#include <string>

template <typename Resource>
class ResourceManager {
public:
	std::shared_ptr<Resource> load(const std::string& filename);

	void unload(const std::string& filename);
	void unload_all();

private:
	std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
};