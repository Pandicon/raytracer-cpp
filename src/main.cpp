#include <ctime>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

#include <SDL2/SDL.h>

#include "Colour.hpp"
#include "materials/Light.hpp"
#include "materials/Diffuse.hpp"
#include "materials/Metal.hpp"
#include "materials/Mirror.hpp"
#include "Scene.hpp"
#include "SceneBuilder.hpp"
#include "shapes/Composite.hpp"
#include "shapes/Quadrilateral.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Triangle.hpp"

#include "save_to_image.hpp"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;
constexpr double FRAMES_PER_LOOP = 10.0;
constexpr std::optional<std::string> FILE_TO_LOAD = std::nullopt;
constexpr std::optional<unsigned int> THREADS_TO_USE_OVERRIDE = std::nullopt;

std::vector<uint32_t> apply_tonemapping_and_pack(const std::vector<ColourXYZ> &accumulated_image, double frames_accumulated);

int main(int argc, char *argv[])
{
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
    {
        num_threads = 2;
    }
    if (THREADS_TO_USE_OVERRIDE.has_value())
    {
        num_threads = THREADS_TO_USE_OVERRIDE.value();
    }
    std::cout << "Using " << num_threads << " thread(s) for rendering" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    bool is_running = true;
    SDL_Event event;

    SceneBuilder scene_builder = SceneBuilder::metal_spheres();
    /*{
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Sphere sphere = Sphere(Vec3(-1.6, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::silver(1.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(-0.8, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::silver(0.5);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.0, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::silver(0.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.8, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::copper(1.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(-0.8, 0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::copper(0.5);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.0, 0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::copper(0.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.8, 0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::gold(1.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(-0.8, 1.6, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::gold(0.5);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.0, 1.6, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::gold(0.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.8, 1.6, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Mirror mirror_material = Mirror(ColourRGB::fromRGB(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(mirror_material);
        Sphere sphere = Sphere(Vec3(1.6, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Dielectric glass_material = Dielectric(ColourRGB::fromRGB(255, 255, 255, 255), 1.5);
        uint32_t material_id = scene_builder.add_material(glass_material);
        Sphere sphere = Sphere(Vec3(0.0, -1.0, 6.0), 0.5, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Dielectric air_material = Dielectric(ColourRGB::fromRGB(255, 255, 255, 255), 1.0);
        uint32_t material_id = scene_builder.add_material(air_material);
        Sphere sphere = Sphere(Vec3(0.0, -1.0, 6.0), 0.4, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Dielectric glass_material = Dielectric(ColourRGB::fromRGB(255, 255, 255, 255), 1.5);
        uint32_t material_id = scene_builder.add_material(glass_material);
        Sphere sphere = Sphere(Vec3(1.2, -1.0, 6.0), 0.5, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    double min_r = 0.0001;
    double max_r = 0.05;
    double min_x = -0.3;
    double max_x = 0.3;
    double min_y = 0.0;
    double max_y = 0.0;
    uint32_t num = 100000;
    for (uint32_t i = 0; i < num; i += 1)
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(0, 0, 255, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Sphere sphere = Sphere(Vec3(max_x - ((double)i / (double)num) * (max_x - min_x), max_y - ((double)i / (double)num) * (max_y - min_y), 5.0), max_r - ((double)i / (double)num) * (max_r - min_r), material_id);
        scene_builder.add_object(std::move(sphere));
    }*/
    /*{
        // Dielectric glass_material = Dielectric(ColourRGB::fromRGB(255, 255, 255), {1.03961212, 0.231792344, 1.01046945}, {0.00600069867, 0.0200179144, 103.560653}, scene_builder.rgb2spec);
        Dielectric glass_material = Dielectric(ColourRGB::fromRGB(255, 255, 255), {1.73759695, 0.313747346, 1.89878101}, {0.013188707, 0.0623068142, 155.23629}, scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(glass_material);
        Sphere sphere = Sphere(Vec3(0.45, -0.75, 6.35), 0.75, material_id);
        scene_builder.add_object(std::move(sphere));
    }*/
    /*{
        Mirror mirror_material = Mirror(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(mirror_material);
        Triangle bottom_tile_1 = Triangle(Vec3(2.1, -2.0, 8.1), Vec3(0.0, 0.0, -4.2), Vec3(-4.2, 0.0, 0.0), material_id);
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        material_id = scene_builder.add_material(diffuse_material);
        bottom_tile_1.set_material(material_id);
        Triangle bottom_tile_2 = Triangle::from_vertices(Vec3(-2.1, -2.0, 3.9), Vec3(2.1, -2.0, 3.9), Vec3(-2.1, -2.0, 8.1), material_id);
        std::vector<SceneObject> floor_objects;
        floor_objects.push_back(std::move(bottom_tile_1));
        floor_objects.push_back(std::move(bottom_tile_2));
        auto floor = std::make_unique<Composite>(std::move(floor_objects));
        box_objects.push_back(std::move(floor));
    }*/

    Scene scene = scene_builder.build();

    double frames_accumulated = 0.0;
    std::vector<ColourXYZ> accumulated_image(WIDTH * HEIGHT, ColourXYZ(0.0, 0.0, 0.0));
    if (FILE_TO_LOAD.has_value())
    {
        load_data(accumulated_image, frames_accumulated, FILE_TO_LOAD.value());
    }
    double last_frame = (double)SDL_GetTicks64();

    while (is_running)
    {
        double this_frame = (double)SDL_GetTicks64();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    is_running = false;
                    break;

                case SDLK_s:
                {
                    std::vector<uint32_t> accumulated_pixels =
                        apply_tonemapping_and_pack(accumulated_image, frames_accumulated);
                    save_texture(accumulated_pixels, WIDTH, HEIGHT, std::to_string(std::time(nullptr)));
                    save_data(accumulated_image, frames_accumulated, std::to_string(std::time(nullptr)));
                    break;
                }

                default:
                    break;
                }
            }
        }

        frames_accumulated += FRAMES_PER_LOOP;

        scene.render(accumulated_image, WIDTH, HEIGHT, num_threads, frames_accumulated, FRAMES_PER_LOOP);

        std::vector<uint32_t> accumulated_pixels = apply_tonemapping_and_pack(accumulated_image, frames_accumulated);

        std::cout << 1000.0 / (this_frame - last_frame) * FRAMES_PER_LOOP << " FPS (average of " << 1000.0 * frames_accumulated / this_frame << ")" << std::endl;

        SDL_UpdateTexture(texture, nullptr, accumulated_pixels.data(), WIDTH * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        last_frame = this_frame;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

constexpr double GAMMA = 2.2;

std::vector<uint32_t> apply_tonemapping_and_pack(const std::vector<ColourXYZ> &accumulated_image, double frames_accumulated)
{
    return accumulated_image | std::views::transform([frames_accumulated](ColourXYZ c_accumulated_xyz)
                                                     {
                                                const ColourRGB c_accumulated = c_accumulated_xyz.to_rgb();
                                                const ColourRGB c = c_accumulated / frames_accumulated;
                                                const ColourRGB c_safe = ColourRGB(std::max(0.0, c.r), std::max(0.0, c.g), std::max(0.0, c.b));
                                                const double max_channel = std::max(0.0, std::max(c_safe.r, std::max(c_safe.g, c_safe.b)));
                                                const double scale = 1.0 / (max_channel + 1.0);
                                                const ColourRGB tonemapped = c_safe * scale;
                                                //const ColourRGB tonemapped = ColourRGB(c.r / (c.r + 1.0), c.g / (c.g + 1.0), c.b / (c.b + 1.0));
                                                const ColourRGB gamma_corrected = ColourRGB(std::pow(tonemapped.r, 1.0 / GAMMA), std::pow(tonemapped.g, 1.0 / GAMMA), std::pow(tonemapped.b, 1.0 / GAMMA));
                                                return gamma_corrected.pack(255); }) |
           std::ranges::to<std::vector>();
}