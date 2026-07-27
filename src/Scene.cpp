#include <algorithm>
#include <atomic>
#include <cmath>
#include <thread>
#include <stack>

#include "BVH.hpp"
#include "Colour.hpp"
#include "Hittable.hpp"
#include "Scene.hpp"

#include "limits.hpp"
#include "matching.hpp"

constexpr int MAX_BOUNCES = 10;

constexpr uint32_t CHUNK_SIZE = 4;

constexpr double G = 1.32471795724474602596;
constexpr double a1 = 1.0 / G;
constexpr double a2 = 1.0 / (G * G);

void run_render_thread(std::vector<ColourXYZ> &accumulated_pixels, std::atomic<uint32_t> &next_row, uint32_t width, uint32_t height, const std::vector<Hittable> &objects, const std::vector<Material> &materials, const BVH &bvh, double void_index_of_refraction, double frame_number, double frames_per_loop);

Scene::Scene(std::vector<Hittable> objects, std::vector<Material> materials, double void_index_of_refraction, BVH bvh, RGB2Spec *rgb2spec) : objects_(std::move(objects)), materials_(std::move(materials)), void_index_of_refraction_(void_index_of_refraction), bvh_(bvh), rgb2spec_(rgb2spec)
{
}

Scene::~Scene()
{
    if (rgb2spec_)
    {
        rgb2spec_free(rgb2spec_);
    }
}

void Scene::add_object(const Hittable &object)
{
    objects_.push_back(object);
}

uint32_t Scene::add_material(const Material &material)
{
    materials_.push_back(material);
    return materials_.size() - 1;
}

void Scene::render(std::vector<ColourXYZ> &accumulated_pixels, uint32_t width, uint32_t height, uint32_t n_threads, double frame_number, double frames_per_loop)
{
    std::atomic<uint32_t> next_row{0};
    std::vector<std::jthread> threads;
    for (int thread_id = 0; thread_id < n_threads; thread_id += 1)
    {
        threads.emplace_back(run_render_thread, std::ref(accumulated_pixels), std::ref(next_row), width, height, std::cref(objects_), std::cref(materials_), bvh_, void_index_of_refraction_, frame_number, frames_per_loop);
    }
}

void run_render_thread(std::vector<ColourXYZ> &accumulated_pixels, std::atomic<uint32_t> &next_row, uint32_t width, uint32_t height, const std::vector<Hittable> &objects, const std::vector<Material> &materials, const BVH &bvh, double void_index_of_refraction, double frame_number, double frames_per_loop)
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
                ColourXYZ pixel_accumulator = ColourXYZ(0.0, 0.0, 0.0);
                for (uint32_t f_i = 0; f_i < frames_per_loop; f_i += 1)
                {
                    double lambda = get_random_double(Limits::MIN_LAMBDA_NM, Limits::MAX_LAMBDA_NM);
                    std::stack<double> indices_of_refraction;

                    double r2_x = precomputed_r2_x[f_i];
                    double r2_y = precomputed_r2_y[f_i];

                    double x_d = (double)x + r2_x;
                    double y_d = (double)y + r2_y;

                    Ray ray = Ray(Vec3(0.0, 0.0, 0.0), Vec3((x_d - w_d / 2.0) / h_d, (height / 2.0 - y_d) / h_d, 1.0).normalise());
                    std::optional<Ray> ray_opt = ray;
                    int bounces = 0;
                    double final_pixel_intensity = 0.0; // Needs to be gathered by hitting lights
                    double ray_colour = 1.0;            // What is currently being carried around, gets lost at bounces
                    while (ray_opt && bounces <= MAX_BOUNCES)
                    {
                        Ray ray = *ray_opt;
                        /*std::optional<HitRecord> closest_hit = {};
                        for (const auto &object : objects)
                        {
                            std::visit([&](const auto &concrete_obj)
                                       {
                                if (auto result = concrete_obj.hit(ray))
                                {
                                    if (ray.valid_range.contains_value(result->t))
                                    {
                                        ray.valid_range.max = result->t;
                                        closest_hit = result;
                                    }
                                } }, object);
                        }*/

                        std::optional<HitRecord> closest_hit = bvh.intersect_with_ray(ray, objects);

                        if (closest_hit)
                        {
                            const uint32_t hit_material_id = closest_hit->material_id_;
                            const Material &hit_material = materials[hit_material_id];

                            double emitted = 0.0;
                            double colour_albedo = 1.0;
                            std::visit(overloaded{[&](const Dielectric &dielectric)
                                                  {
                                                      emitted = dielectric.emitted(ray);
                                                      colour_albedo = dielectric.albedo(ray, lambda);

                                                      if (!closest_hit->front_face)
                                                      {
                                                          // Leaving the object (but may internally reflect)
                                                          indices_of_refraction.pop(); // Remove the current index of refraction
                                                          double n_previous = dielectric.index_of_refraction(lambda);
                                                          double n_next = void_index_of_refraction;
                                                          if (!indices_of_refraction.empty())
                                                          {
                                                              n_next = indices_of_refraction.top();
                                                          }
                                                          bool refracted = true;
                                                          ray_opt = dielectric.scatter(ray, closest_hit->point, closest_hit->normal, n_previous / n_next, refracted);
                                                          if (!refracted)
                                                          {
                                                              // We did not actually leave the medium
                                                              indices_of_refraction.push(dielectric.index_of_refraction(lambda));
                                                          }
                                                      }
                                                      else
                                                      {
                                                          // Entering the object (but may reflect)
                                                          double n_previous = void_index_of_refraction;
                                                          if (!indices_of_refraction.empty())
                                                          {
                                                              n_previous = indices_of_refraction.top();
                                                          }
                                                          double n_next = dielectric.index_of_refraction(lambda);
                                                          bool refracted = true;
                                                          ray_opt = dielectric.scatter(ray, closest_hit->point, closest_hit->normal, n_previous / n_next, refracted);
                                                          if (refracted)
                                                          {
                                                              // We entered the medium
                                                              indices_of_refraction.push(dielectric.index_of_refraction(lambda));
                                                          }
                                                      }
                                                  },
                                                  [&](const Metal &metal)
                                                  {
                                                      /*double n_previous = void_index_of_refraction;
                                                      if (!indices_of_refraction.empty())
                                                      {
                                                          n_previous = indices_of_refraction.top();
                                                      }
                                                      emitted = metal.emitted(ray);

                                                      std::optional<ScatterRecord> scatter_result = metal.scatter(ray, closest_hit->point, closest_hit->normal, n_previous);
                                                      if (scatter_result)
                                                      {
                                                          colour_albedo = scatter_result->colour_albedo;
                                                          ray_opt = scatter_result->scattered_ray;
                                                      }*/
                                                      throw std::runtime_error("Metals are not implemented yet");
                                                  },
                                                  [&](const Light &light)
                                                  {
                                                      emitted = light.emitted(ray, lambda);

                                                      colour_albedo = light.albedo(ray);

                                                      ray_opt = light.scatter(ray, closest_hit->point, closest_hit->normal);
                                                  },
                                                  [&](const BlackBody &bb)
                                                  {
                                                      emitted = bb.emitted(ray, lambda);

                                                      colour_albedo = bb.albedo(ray);

                                                      ray_opt = bb.scatter(ray, closest_hit->point, closest_hit->normal);
                                                  },
                                                  [&](const Diffuse &diffuse)
                                                  {
                                                      emitted = diffuse.emitted(ray);

                                                      colour_albedo = diffuse.albedo(ray, lambda);

                                                      ray_opt = diffuse.scatter(ray, closest_hit->point, closest_hit->normal);
                                                  },
                                                  [&](const Mirror &mirror)
                                                  {
                                                      emitted = mirror.emitted(ray);

                                                      colour_albedo = mirror.albedo(ray, lambda);

                                                      ray_opt = mirror.scatter(ray, closest_hit->point, closest_hit->normal);
                                                  }},
                                       hit_material);
                            final_pixel_intensity = final_pixel_intensity + ray_colour * emitted;
                            ray_colour = ray_colour * colour_albedo;
                            bounces += 1;
                        }
                        else
                        {
                            ray_opt = std::nullopt;
                        }
                    }
                    pixel_accumulator = pixel_accumulator + ColourXYZ::from_lambda_intensity(lambda, final_pixel_intensity, Limits::MAX_LAMBDA_NM - Limits::MIN_LAMBDA_NM);
                }
                accumulated_pixels[pixel_index] = accumulated_pixels[pixel_index] + pixel_accumulator;
            }
        }
    }
}