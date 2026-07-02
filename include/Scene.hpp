#pragma once

#include <memory>
#include <vector>

#include "Hittable.hpp"

class Scene
{
private:
    std::vector<std::unique_ptr<Hittable>> objects_;

public:
    Scene() = default;
    Scene(std::vector<std::unique_ptr<Hittable>> objects);
    void render(std::vector<Colour> &accumulated_pixels, uint32_t width, uint32_t height, uint32_t n_threads, double frame_number, double frames_per_loop);

    void add_object(std::unique_ptr<Hittable> object);
};