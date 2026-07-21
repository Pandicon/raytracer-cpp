#include <iostream>
#include <memory>
#include <ranges>
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

const int WIDTH = 800;
const int HEIGHT = 800;

std::vector<uint32_t> apply_tonemapping_and_pack(const std::vector<Colour> &accumulated_image, double frames_accumulated);

int main(int argc, char *argv[])
{
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
    {
        num_threads = 2;
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

    SceneBuilder scene_builder = SceneBuilder(1.0);
    {
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(255, 255, 255, 255));
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
        Mirror mirror_material = Mirror(Colour::fromRGBA(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(mirror_material);
        Sphere sphere = Sphere(Vec3(1.6, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Dielectric glass_material = Dielectric(Colour::fromRGBA(255, 255, 255, 255), 1.5);
        uint32_t material_id = scene_builder.add_material(glass_material);
        Sphere sphere = Sphere(Vec3(0.0, -1.0, 6.0), 0.5, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Dielectric air_material = Dielectric(Colour::fromRGBA(255, 255, 255, 255), 1.0);
        uint32_t material_id = scene_builder.add_material(air_material);
        Sphere sphere = Sphere(Vec3(0.0, -1.0, 6.0), 0.4, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Dielectric glass_material = Dielectric(Colour::fromRGBA(255, 255, 255, 255), 1.5);
        uint32_t material_id = scene_builder.add_material(glass_material);
        Sphere sphere = Sphere(Vec3(1.2, -1.0, 6.0), 0.5, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    std::vector<SceneObject> box_objects;
    {
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(0, 255, 0, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral right_wall = Quadrilateral(Vec3(2.0, -2.1, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(0.0, 4.2, 0.0), material_id);
        box_objects.push_back(std::move(right_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(255, 0, 0, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral left_wall = Quadrilateral(Vec3(-2.0, -2.1, 3.9), Vec3(0.0, 4.2, 0.0), Vec3(0.0, 0.0, 4.2), material_id);
        box_objects.push_back(std::move(left_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral back_wall = Quadrilateral(Vec3(-2.1, -2.1, 8.0), Vec3(0.0, 4.2, 0.0), Vec3(4.2, 0.0, 0.0), material_id);
        box_objects.push_back(std::move(back_wall));
    }
    /*{
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral bottom_wall = Quadrilateral(Vec3(-2.1, -2.0, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(4.2, 0.0, 0.0), material_id);
        scene_builder.add_object(std::move(bottom_wall));
    }*/
    {
        Mirror mirror_material = Mirror(Colour::fromRGBA(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(mirror_material);
        Triangle bottom_tile_1 = Triangle(Vec3(2.1, -2.0, 8.1), Vec3(0.0, 0.0, -4.2), Vec3(-4.2, 0.0, 0.0), material_id);
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(255, 255, 255, 255));
        material_id = scene_builder.add_material(diffuse_material);
        bottom_tile_1.set_material(material_id);
        Triangle bottom_tile_2 = Triangle::from_vertices(Vec3(-2.1, -2.0, 3.9), Vec3(2.1, -2.0, 3.9), Vec3(-2.1, -2.0, 8.1), material_id);
        std::vector<SceneObject> floor_objects;
        floor_objects.push_back(std::move(bottom_tile_1));
        floor_objects.push_back(std::move(bottom_tile_2));
        auto floor = std::make_unique<Composite>(std::move(floor_objects));
        box_objects.push_back(std::move(floor));
    }
    {
        Diffuse diffuse_material = Diffuse(Colour::fromRGBA(255, 255, 255, 255));
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral top_wall = Quadrilateral(Vec3(-2.1, 2.0, 3.9), Vec3(4.2, 0.0, 0.0), Vec3(0.0, 0.0, 4.2), material_id);
        box_objects.push_back(std::move(top_wall));
    }
    {
        Light light_material = Light(Colour::fromRGBA(255, 255, 255, 255), 2.0);
        uint32_t material_id = scene_builder.add_material(light_material);
        Quadrilateral top_light = Quadrilateral(Vec3(-0.5, 1.999, 5.5), Vec3(1.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0), material_id);
        box_objects.push_back(std::move(top_light));
    }
    auto box = std::make_unique<Composite>(std::move(box_objects));
    scene_builder.add_object(std::move(box));

    Scene scene = scene_builder.build();

    double frames_accumulated = 0.0;
    std::vector<Colour> accumulated_image(WIDTH * HEIGHT, Colour(0.0, 0.0, 0.0, 0.0));
    double last_frame = (double)SDL_GetTicks64();
    double frames_per_loop = 10.0;
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
                    save_texture(accumulated_pixels, WIDTH, HEIGHT);
                    break;
                }

                default:
                    break;
                }
            }
        }

        frames_accumulated += frames_per_loop;

        scene.render(accumulated_image, WIDTH, HEIGHT, num_threads, frames_accumulated, frames_per_loop);

        std::vector<uint32_t> accumulated_pixels = apply_tonemapping_and_pack(accumulated_image, frames_accumulated);

        std::cout << 1000.0 / (this_frame - last_frame) * frames_per_loop << " FPS (average of " << 1000.0 * frames_accumulated / this_frame << ")" << std::endl;

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

std::vector<uint32_t> apply_tonemapping_and_pack(const std::vector<Colour> &accumulated_image, double frames_accumulated)
{
    return accumulated_image | std::views::transform([frames_accumulated](Colour c_accumulated)
                                                     {
                                                const Colour c = c_accumulated / frames_accumulated;
                                                const Colour tonemapped = Colour(c.r / (c.r + 1.0), c.g / (c.g + 1.0), c.b / (c.b + 1.0), c.a);
                                                const Colour gamma_corrected = Colour(std::pow(tonemapped.r, 1.0 / GAMMA), std::pow(tonemapped.g, 1.0 / GAMMA), std::pow(tonemapped.b, 1.0 / GAMMA), tonemapped.a);
                                                return gamma_corrected.pack(); }) |
           std::ranges::to<std::vector>();
}