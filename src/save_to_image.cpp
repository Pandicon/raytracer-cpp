#include <ctime>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "save_to_image.hpp"

void save_texture(const std::vector<uint32_t> &pixels, int width, int height)
{
    std::vector<uint8_t> rgba;
    rgba.reserve(width * height * 4);

    for (uint32_t p : pixels)
    {
        uint8_t a = (p >> 24) & 0xFF;
        uint8_t r = (p >> 16) & 0xFF;
        uint8_t g = (p >> 8) & 0xFF;
        uint8_t b = p & 0xFF;

        rgba.push_back(r);
        rgba.push_back(g);
        rgba.push_back(b);
        rgba.push_back(a);
    }

    std::string filename = std::to_string(std::time(nullptr)) + ".png";

    stbi_write_png(
        filename.c_str(),
        width,
        height,
        4,
        rgba.data(),
        4 * width);
}