#include <algorithm>
#include <atomic>
#include <cmath>
#include <thread>

#include "Colour.hpp"
#include "Hittable.hpp"
#include "Scene.hpp"

const double MINIMUM_T = 0.0001;
const int MAX_BOUNCES = 10;

const uint32_t CHUNK_SIZE = 4;

const double G = 1.32471795724474602596;
const double a1 = 1.0 / G;
const double a2 = 1.0 / (G * G);

void run_render_thread(std::vector<Colour> &accumulated_pixels, std::atomic<uint32_t> &next_row, uint32_t width, uint32_t height, const std::vector<Hittable> &objects_, double frame_number, double frames_per_loop);

Scene::Scene(std::vector<Hittable> objects) : objects_(objects) {}

void Scene::add_object(const Hittable &object)
{
    objects_.push_back(object);
}

void Scene::render(std::vector<Colour> &accumulated_pixels, uint32_t width, uint32_t height, uint32_t n_threads, double frame_number, double frames_per_loop)
{
    std::atomic<uint32_t> next_row{0};
    std::vector<std::jthread> threads;
    for (int thread_id = 0; thread_id < n_threads; thread_id += 1)
    {
        threads.emplace_back(run_render_thread, std::ref(accumulated_pixels), std::ref(next_row), width, height, std::cref(objects_), frame_number, frames_per_loop);
    }
}

void run_render_thread(std::vector<Colour> &accumulated_pixels, std::atomic<uint32_t> &next_row, uint32_t width, uint32_t height, const std::vector<Hittable> &objects_, double frame_number, double frames_per_loop)
{
    std::vector<double> precomputed_r2_x(frames_per_loop);
    std::vector<double> precomputed_r2_y(frames_per_loop);

    for (uint32_t f_i = 0; f_i < frames_per_loop; f_i += 1)
    {
        double current_frame = frame_number + (double)f_i;
        precomputed_r2_x[f_i] = std::fmod(0.5 + a1 * current_frame, 1.0);
        precomputed_r2_y[f_i] = std::fmod(0.5 + a2 * current_frame, 1.0);
    }
    double w_d = (double)width;
    double h_d = (double)height;
    while (true)
    {
        uint32_t start_y = next_row.fetch_add(CHUNK_SIZE);
        uint32_t end_y = std::min(start_y + CHUNK_SIZE, height);
        if (start_y >= height)
        {
            break;
        }
        for (uint32_t y = start_y; y < end_y; y += 1)
        {
            for (uint32_t x = 0; x < width; x += 1)
            {
                uint32_t pixel_index = y * width + x;
                Colour pixel_accumulator = Colour::black();
                for (uint32_t f_i = 0; f_i < frames_per_loop; f_i += 1)
                {
                    double r2_x = precomputed_r2_x[f_i];
                    double r2_y = precomputed_r2_y[f_i];

                    double x_d = (double)x + r2_x;
                    double y_d = (double)y + r2_y;

                    Ray ray = Ray(Vec3(0.0, 0.0, 0.0), Vec3((x_d - w_d / 2.0) / h_d, (height / 2.0 - y_d) / h_d, 1.0).normalise());
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
                            std::visit([&](const auto &concrete_obj)
                                       {
                                if (auto result = concrete_obj.hit(ray))
                                {
                                    if (result->t > MINIMUM_T && result->t < closest_t)
                                    {
                                        closest_t = result->t;
                                        closest_hit = result;
                                    }
                                } }, object);
                        }

                        if (closest_hit)
                        {
                            const Material *hit_material = closest_hit->material;
                            std::visit([&](const auto &concrete_material)
                                       {
                                Colour emitted = concrete_material.emitted(ray);
                                final_pixel_colour = final_pixel_colour + ray_colour * emitted;

                                Colour colour_albedo = concrete_material.colour_contribution(ray);
                                ray_colour = ray_colour * colour_albedo;

                                ray_opt = concrete_material.scatter(ray, closest_hit->point, closest_hit->normal);
                                bounces += 1; }, *hit_material);
                        }
                        else
                        {
                            ray_opt = std::nullopt;
                        }
                    }
                    pixel_accumulator = pixel_accumulator + final_pixel_colour;
                }
                accumulated_pixels[pixel_index] = accumulated_pixels[pixel_index] + pixel_accumulator;
            }
        }
    }
}