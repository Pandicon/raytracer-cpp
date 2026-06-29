#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

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

    std::vector<uint32_t> pixels(WIDTH * HEIGHT, 0);

    bool is_running = true;
    SDL_Event event;

    uint32_t gradient_offset = 0;

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

        gradient_offset += 1;
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                uint8_t r = (x + gradient_offset) % 256;
                uint8_t g = (y + gradient_offset) % 256;
                uint8_t b = 128;
                pixels[y * WIDTH + x] = (255 << 24) | (r << 16) | (g << 8) | b;
            }
        }

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(uint32_t));

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