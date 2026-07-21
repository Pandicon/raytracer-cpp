#pragma once

#include <memory>
#include <vector>

#include "Hittable.hpp"
#include "Material.hpp"

class Scene
{
    friend class SceneBuilder;

private:
    std::vector<Hittable> objects_;
    std::vector<Material> materials_;
    double void_index_of_refraction_;
    Scene(std::vector<Hittable> objects, std::vector<Material> materials, double void_index_of_refraction);

public:
    Scene() = default;
    Scene(double void_index_of_refraction);
    Scene(std::vector<Hittable> objects, double void_index_of_refraction);
    void render(std::vector<Colour> &accumulated_pixels, uint32_t width, uint32_t height, uint32_t n_threads, double frame_number, double frames_per_loop);

    void add_object(const Hittable &object);
    uint32_t add_material(const Material &material);
};