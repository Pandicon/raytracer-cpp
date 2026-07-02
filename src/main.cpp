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
#include "Quadrilateral.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"

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

    Scene scene = Scene();
    {
        std::shared_ptr<Diffuse> diffuse_material = std::make_shared<Diffuse>(Colour::fromRGBA(255, 255, 255, 255));
        Sphere sphere = Sphere(Vec3(-1.6, 0.0, 7.0), 0.3, diffuse_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }
    {
        std::shared_ptr<Metal> metal_material = std::make_shared<Metal>(Colour::fromRGBA(255, 255, 255, 255), 1.0, 1.0);
        Sphere sphere = Sphere(Vec3(-0.8, 0.0, 7.0), 0.3, metal_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }
    {
        std::shared_ptr<Metal> metal_material = std::make_shared<Metal>(Colour::fromRGBA(255, 255, 255, 255), 1.0, 0.5);
        Sphere sphere = Sphere(Vec3(0.0, 0.0, 7.0), 0.3, metal_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }
    {
        std::shared_ptr<Metal> metal_material = std::make_shared<Metal>(Colour::fromRGBA(255, 255, 255, 255), 1.0, 0.0);
        Sphere sphere = Sphere(Vec3(0.8, 0.0, 7.0), 0.3, metal_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }
    {
        std::shared_ptr<Mirror> mirror_material = std::make_shared<Mirror>(Colour::fromRGBA(255, 255, 255, 255));
        Sphere sphere = Sphere(Vec3(1.6, 0.0, 7.0), 0.3, mirror_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }
    {
        std::shared_ptr<Diffuse> diffuse_material = std::make_shared<Diffuse>(Colour::fromRGBA(0, 255, 0, 255));
        Quadrilateral right_wall = Quadrilateral(Vec3(2.0, -2.1, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(0.0, 4.2, 0.0), diffuse_material);
        scene.add_object(std::make_unique<Quadrilateral>(right_wall));
    }
    {
        std::shared_ptr<Diffuse> diffuse_material = std::make_shared<Diffuse>(Colour::fromRGBA(255, 0, 0, 255));
        Quadrilateral left_wall = Quadrilateral(Vec3(-2.0, -2.1, 3.9), Vec3(0.0, 4.2, 0.0), Vec3(0.0, 0.0, 4.2), diffuse_material);
        scene.add_object(std::make_unique<Quadrilateral>(left_wall));
    }
    {
        std::shared_ptr<Diffuse> diffuse_material = std::make_shared<Diffuse>(Colour::fromRGBA(255, 255, 255, 255));
        Quadrilateral back_wall = Quadrilateral(Vec3(-2.1, -2.1, 8.0), Vec3(0.0, 4.2, 0.0), Vec3(4.2, 0.0, 0.0), diffuse_material);
        scene.add_object(std::make_unique<Quadrilateral>(back_wall));
    }
    {
        std::shared_ptr<Diffuse> diffuse_material = std::make_shared<Diffuse>(Colour::fromRGBA(255, 255, 255, 255));
        Quadrilateral bottom_wall = Quadrilateral(Vec3(-2.1, -2.0, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(4.2, 0.0, 0.0), diffuse_material);
        scene.add_object(std::make_unique<Quadrilateral>(bottom_wall));
    }
    {
        std::shared_ptr<Diffuse> diffuse_material = std::make_shared<Diffuse>(Colour::fromRGBA(255, 255, 255, 255));
        Quadrilateral top_wall = Quadrilateral(Vec3(-2.1, 2.0, 3.9), Vec3(4.2, 0.0, 0.0), Vec3(0.0, 0.0, 4.2), diffuse_material);
        scene.add_object(std::make_unique<Quadrilateral>(top_wall));
    }
    {
        std::shared_ptr<Light> light_material = std::make_shared<Light>(Colour::fromRGBA(255, 255, 255, 255), 2.0);
        Quadrilateral top_light = Quadrilateral(Vec3(-0.5, 1.999, 5.5), Vec3(1.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0), light_material);
        scene.add_object(std::make_unique<Quadrilateral>(top_light));
    }

    double frames_accumulated = 0.0;
    std::vector<Colour> accumulated_image(WIDTH * HEIGHT, Colour(0.0, 0.0, 0.0, 0.0));
    double last_frame = (double)SDL_GetTicks64();
    std::vector<Colour> pixels(WIDTH * HEIGHT, Colour(0.0, 0.0, 0.0, 0.0));
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

        frames_accumulated += 1.0;

        scene.render(pixels, WIDTH, HEIGHT, num_threads, frames_accumulated);
        for (int i = 0; i < pixels.size(); i += 1)
        {
            accumulated_image[i] = accumulated_image[i] + pixels[i];
        }

        std::vector<uint32_t> accumulated_pixels = apply_tonemapping_and_pack(accumulated_image, frames_accumulated);

        std::cout << 1000.0 / (this_frame - last_frame) << " FPS (average of " << 1000.0 * frames_accumulated / this_frame << ")" << std::endl;

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

const double GAMMA = 2.2;

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