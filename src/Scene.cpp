#include "Colour.hpp"
#include "Scene.hpp"

const double MINIMUM_T = 0.0001;
const int MAX_BOUNCES = 10;

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
            Ray ray = Ray(Vec3(0.0, 0.0, 0.0), Vec3((double(x) - w_d / 2.0) / h_d, (height / 2.0 - (double)y) / h_d, 1.0).normalise());
            std::optional<Ray> ray_opt = ray;
            int bounces = 0;
            Colour accumulated_colour = Colour::white();
            while (ray_opt && bounces <= MAX_BOUNCES)
            {
                Ray ray = *ray_opt;
                double closest_t = std::numeric_limits<double>::max();
                std::optional<HitRecord> closest_hit = {};
                for (const auto &object : objects_)
                {
                    if (auto result = object->hit(ray))
                    {
                        if (result->t > MINIMUM_T && result->t < closest_t)
                        {
                            closest_t = result->t;
                            closest_hit = result;
                        }
                    }
                }

                if (closest_hit)
                {
                    Colour colour_contribution = closest_hit->hit_object.get().colour_contribution(ray);
                    accumulated_colour = accumulated_colour * colour_contribution;
                    ray_opt = closest_hit->hit_object.get().scatter(ray, closest_hit->point, closest_hit->normal);
                    bounces += 1;
                }
                else
                {
                    accumulated_colour = Colour(0.0, 0.0, 0.0, 1.0);
                    ray_opt = std::nullopt;
                }
            }
            if (bounces > MAX_BOUNCES)
            {
                accumulated_colour = Colour(0.0, 0.0, 0.0, 1.0);
            }
            pixels[pixel_index] = accumulated_colour;
        }
    }
}