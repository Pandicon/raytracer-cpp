#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "save_to_image.hpp"

#include "Colour.hpp"

void save_texture(const std::vector<uint32_t> &pixels, int width, int height, const std::string &filename)
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

    std::string filename_with_ext = filename + ".png";

    stbi_write_png(
        filename_with_ext.c_str(),
        width,
        height,
        4,
        rgba.data(),
        4 * width);
}

void save_data(const std::vector<ColourXYZ> &pixels, double accumulated_frames, const std::string &filename)
{
    std::ofstream raw_file = std::ofstream(filename + ".raw");
    raw_file << std::fixed << std::setprecision(10);
    raw_file << accumulated_frames << '\n';
    raw_file << pixels.size() << '\n';
    for (const ColourXYZ &c : pixels)
    {
        raw_file << c.x << " " << c.y << " " << c.z << '\n';
    }
    raw_file.close();
}

void load_data(std::vector<ColourXYZ> &pixels, double &accumulated_frames, const std::string &filename)
{
    std::ifstream raw_file = std::ifstream(filename + ".raw");
    raw_file >> accumulated_frames;
    uint32_t num_pixels;
    raw_file >> num_pixels;
    std::vector<ColourXYZ> extracted_pixels;
    extracted_pixels.reserve(num_pixels);
    for (int i = 0; i < num_pixels; i += 1)
    {
        double x, y, z;
        raw_file >> x >> y >> z;
        extracted_pixels.push_back(ColourXYZ(x, y, z));
    }
    raw_file.close();

    pixels = std::move(extracted_pixels);
}