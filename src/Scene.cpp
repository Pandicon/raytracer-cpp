#include <algorithm>
#include <thread>

#include "Colour.hpp"
#include "Scene.hpp"

const double MINIMUM_T = 0.0001;
const int MAX_BOUNCES = 10;

void run_render_thread(std::vector<Colour> &pixels, uint32_t start_row, uint32_t end_row, uint32_t width, uint32_t height, const std::vector<std::unique_ptr<Hittable>> &objects_);

Scene::Scene(std::vector<std::unique_ptr<Hittable>> objects) : objects_(std::move(objects)) {}

void Scene::add_object(std::unique_ptr<Hittable> object)
{
    objects_.push_back(std::move(object));
}

void Scene::render(std::vector<Colour> &pixels, uint32_t width, uint32_t height, uint32_t n_threads)
{
    uint32_t rows_per_thread = (height + n_threads - 1) / n_threads;
    std::vector<std::jthread> threads;
    for (int thread_id = 0; thread_id < n_threads; thread_id += 1)
    {
        uint32_t start_row = rows_per_thread * thread_id;
        uint32_t end_row = std::min(height, start_row + rows_per_thread);
        threads.emplace_back(run_render_thread, std::ref(pixels), start_row, end_row, width, height, std::cref(objects_));
    }
}

void run_render_thread(std::vector<Colour> &pixels, uint32_t start_row, uint32_t end_row, uint32_t width, uint32_t height, const std::vector<std::unique_ptr<Hittable>> &objects_)
{
    double w_d = (double)width;
    double h_d = (double)height;
    for (uint32_t y = start_row; y < end_row; y += 1)
    {
        for (uint32_t x = 0; x < width; x += 1)
        {
            uint32_t pixel_index = y * width + x;
            Ray ray = Ray(Vec3(0.0, 0.0, 0.0), Vec3((double(x) - w_d / 2.0) / h_d, (height / 2.0 - (double)y) / h_d, 1.0).normalise());
            std::optional<Ray> ray_opt = ray;
            int bounces = 0;
            Colour final_pixel_colour = Colour(0.0, 0.0, 0.0, 1.0); // Needs to be gathered by hitting lights
            Colour ray_colour = Colour::white();                    // What is currently being carried around, gets lost at bounces
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
                    Colour emitted = closest_hit->hit_object.get().emitted(ray);
                    final_pixel_colour = final_pixel_colour + ray_colour * emitted;

                    Colour colour_albedo = closest_hit->hit_object.get().colour_contribution(ray);
                    ray_colour = ray_colour * colour_albedo;

                    ray_opt = closest_hit->hit_object.get().scatter(ray, closest_hit->point, closest_hit->normal);
                    bounces += 1;
                }
                else
                {
                    ray_opt = std::nullopt;
                }
            }
            pixels[pixel_index] = final_pixel_colour;
        }
    }
}