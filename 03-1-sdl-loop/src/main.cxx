#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string_view>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

struct key_name
{
    SDL_Keycode      key;
    std::string_view name;
};

void print_key(const SDL_Event& event)
{
    using namespace std;
    const array<key_name, 4> keys{ { { SDLK_UP, "Arrow up" },
                                     { SDLK_LEFT, "Arrow left" },
                                     { SDLK_RIGHT, "Arrow right" },
                                     { SDLK_DOWN, "Arrow down" } } };

    const auto it = find_if(begin(keys),
                            end(keys),
                            [&](const key_name& k)
                            { return k.key == event.key.keysym.sym; });

    if (it != end(keys))
    {
        cout << it->name << " ";
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            cout << "is pressed" << endl;
        }
        else
        {
            cout << "is released" << endl;
        }
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL. Error: " << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Loop", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        std::cerr << "Failed to create window. Error: " << SDL_GetError()
                  << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool      loop = true;

    while (loop)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_EVENT_KEY_DOWN:
                    [[fallthrough]];
                case SDL_EVENT_KEY_UP:
                    print_key(e);
                    break;
                case SDL_EVENT_QUIT:
                    loop = false;
                    break;
                default:
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
