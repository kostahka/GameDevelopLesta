#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

int main()
{
    SDL_Window*  window        = nullptr;
    SDL_Surface* screenSurface = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize sdl video error. Error: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Hello SDL", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == nullptr)
    {
        std::cerr << "Failed to create window. Error: " << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }

    screenSurface = SDL_GetWindowSurface(window);

    unsigned char colorGreen = 0;
    SDL_Event     e;
    bool          quit = false;
    while (!quit)
    {
        colorGreen++;

        SDL_FillSurfaceRect(
            screenSurface,
            nullptr,
            SDL_MapRGB(screenSurface->format, 155, colorGreen, 15));
        SDL_UpdateWindowSurface(window);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
