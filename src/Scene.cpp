#include "Colour.hpp"
#include "Scene.hpp"

Scene::Scene(std::vector<std::unique_ptr<Hittable>> objects) : objects_(std::move(objects)) {}

void Scene::add_object(std::unique_ptr<Hittable> object)
{
    objects_.push_back(std::move(object));
}

void Scene::render(std::vector<Colour> &pixels, uint32_t width, uint32_t height)
{
    double w_d = (double)width;
    double h_d = (double)height;
    for (uint32_t x = 0; x < width; x += 1)
    {
        for (uint32_t y = 0; y < height; y += 1)
        {
            uint32_t pixel_index = y * width + x;
            double closest_t = std::numeric_limits<double>::max();
            std::optional<HitRecord> closest_hit = {};
            Ray ray = Ray(Vec3(0.0, 0.0, 0.0), Vec3((double(x) - w_d / 2.0) / h_d, (height / 2.0 - (double)y) / h_d, 1.0).normalise());
            for (const auto &object : objects_)
            {
                if (auto result = object->hit(ray))
                {
                    if (result->t < closest_t)
                    {
                        closest_t = result->t;
                        closest_hit = result;
                    }
                }
            }

            if (auto result = closest_hit)
            {
                Colour pixel_colour = Colour::fromRGBA(130, 0, 73, 255);
                pixels[pixel_index] = pixel_colour;
            }
            else
            {
                pixels[pixel_index] = Colour(0.0, 0.0, 0.0, 1.0);
            }
        }
    }
}