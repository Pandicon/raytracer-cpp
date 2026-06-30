#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

#include <SDL2/SDL.h>

#include "Colour.hpp"
#include "materials/Light.hpp"
#include "materials/Diffuse.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int argc, char *argv[])
{
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
        Sphere sphere = Sphere(Vec3(0.0, 0.0, 5.0), 2.0, diffuse_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }
    {
        std::shared_ptr<Light> light_material = std::make_shared<Light>(Colour::fromRGBA(13, 120, 73, 255), 100.0);
        Sphere sphere = Sphere(Vec3(0.0, 3.0, -8.0), 1.0, light_material);
        scene.add_object(std::make_unique<Sphere>(sphere));
    }

    double frames_accumulated = 0;
    std::vector<Colour> accumulated_image(WIDTH * HEIGHT, Colour(0.0, 0.0, 0.0, 0.0));
    while (is_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    is_running = false;
                }
            }
        }

        frames_accumulated += 1.0;

        std::vector<Colour> pixels(WIDTH * HEIGHT, Colour(0.0, 0.0, 0.0, 0.0));
        scene.render(pixels, WIDTH, HEIGHT);
        for (int i = 0; i < pixels.size(); i += 1)
        {
            accumulated_image[i] = accumulated_image[i] * ((frames_accumulated - 1.0) / (frames_accumulated)) + pixels[i] * (1.0 / frames_accumulated);
        }

        std::vector<uint32_t> accumulated_pixels = accumulated_image | std::views::transform([](Colour c)
                                                                                             { return c.pack(); }) |
                                                   std::ranges::to<std::vector>();

        SDL_UpdateTexture(texture, nullptr, accumulated_pixels.data(), WIDTH * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}