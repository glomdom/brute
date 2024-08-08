#include "brute.hpp"

#include <iostream>

Brute::Brute(const Config& config) : config(config) {}

std::shared_ptr<Texture> Brute::load_texture(const std::string& filename) {
	return textureManager.load(filename);
}

void Brute::run() {
	init_window();
	init_vulkan();
	main_loop();
	cleanup();
}

void Brute::init_window() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, true);

	window = glfwCreateWindow(config.windowWidth, config.windowHeight, config.windowTitle.c_str(), nullptr, nullptr);
	if (!window) {
		throw std::runtime_error("failed to create glfw window");
	}
}

void Brute::init_vulkan() {
	create_instance();

	if (config.enableValidationLayers) {
		setup_debug_messenger();
	}
}

void Brute::create_instance() {
	if (config.enableValidationLayers && !check_validation_layer_support()) {
		throw std::runtime_error("validation layers requested, but aren't available");
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
		createInfo.pNext = (vk::DebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	instance = vk::createInstance(createInfo);

	dldi.init(instance, vkGetInstanceProcAddr);
}

void Brute::setup_debug_messenger() {
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

std::vector<const char*> Brute::get_required_extensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	
	if (config.enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool Brute::check_validation_layer_support() {
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

void Brute::main_loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void Brute::cleanup() {
	if (config.enableValidationLayers) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}

	textureManager.cleanup();

	if (instance) {
		instance.destroy();
	}

	if (window) {
		glfwDestroyWindow(window);
	}

	glfwTerminate();
}