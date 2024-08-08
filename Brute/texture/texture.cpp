#define STB_IMAGE_IMPLEMENTATION
#include <vendor/stb_image.h>

#include "texture.hpp"

bool Texture::load_from_file(const std::string& filename) {
	data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	return data != nullptr;
}

void Texture::cleanup() {
	if (data) {
		stbi_image_free(data);

		data = nullptr;
	}
}