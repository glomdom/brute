#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "config.hpp"
#include "resource_manager.hpp"
#include "texture.hpp"

class Brute {
public:
	Brute(const Config& config);

	void run();

private:
	void init_window();
	void init_vulkan();
	void create_instance();
	void setup_debug_messenger();
	std::vector<const char*> get_required_extensions();
	bool check_validation_layer_support();
	void main_loop();
	void cleanup();

	GLFWwindow* window;
	vk::Instance instance;
	vk::DebugUtilsMessengerEXT debugMessenger;
	vk::DispatchLoaderDynamic dldi;

	Config config;
	ResourceManager<Texture> textureManager;
};