#pragma once

#include <string>

class Texture {
public:
    bool load_from_file(const std::string& filename);
    void cleanup();

    const unsigned char* get_data() const { return data; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_channels() const { return channels; }

private:
    unsigned char* data = nullptr;

    int width = 0;
    int height = 0;
    int channels = 0;
};