#pragma once

#include <vector>

#include "Colour.hpp"

void save_texture(const std::vector<uint32_t> &pixels, int width, int height, const std::string &filename);

void save_data(const std::vector<ColourXYZ> &pixels, double accumulated_frames, const std::string &filename);

void load_data(std::vector<ColourXYZ> &pixels, double &accumulated_frames, const std::string &filename);