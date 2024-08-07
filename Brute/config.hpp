#pragma once

#include "pch.h"

struct Config {
	uint32_t windowWidth = 800;
	uint32_t windowHeight = 600;
	std::string windowTitle = "Brute Default Title";

	bool enableValidationLayers = true;
	std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
};