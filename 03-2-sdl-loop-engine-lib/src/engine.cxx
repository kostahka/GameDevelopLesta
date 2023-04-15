#include "engine.hxx"
#include <SDL3/SDL.h>
#include <SDL_timer.h>
namespace Kengine
{

class engine_impl;
Uint32 update_timer_callback(Uint32 interval, void* param);
Uint32 render_timer_callback(Uint32 interval, void* param);

class engine_impl : public engine
{
public:
    static engine* instance;

    return_code initialize(game* game, std::ostream& error_stream) override
    {
        this->e_game = game;
        this->oerr   = &error_stream;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            *oerr << "Error to initialize SDL. Error: " << SDL_GetError();
            return return_code::sdl_init_fail;
        }

        window = SDL_CreateWindow(game->name.c_str(),
                                  game->configuration.screen_width,
                                  game->configuration.screen_height,
                                  SDL_WINDOW_OPENGL);

        if (window == nullptr)
        {
            *oerr << "Error to create window. Error: " << SDL_GetError();
            SDL_Quit();
            return return_code::create_window_fail;
        }

        renderer = SDL_CreateRenderer(window, "opengl", SDL_RENDERER_SOFTWARE);

        if (renderer == nullptr)
        {
            *oerr << "Error to create renderer. Error: " << SDL_GetError();
            SDL_DestroyWindow(window);
            SDL_Quit();
            return return_code::create_renderer_fail;
        }

        return return_code::good;
    };

    return_code uninitialize() override
    {
        if (renderer)
            SDL_DestroyRenderer(renderer);

        if (window)
            SDL_DestroyWindow(window);

        SDL_Quit();

        return return_code::good;
    };

    return_code start_game_loop() override
    {
        if (e_game == nullptr)
            return return_code::game_not_set;

        bool      continue_loop = true;
        SDL_Event sdl_event;

        e_game->on_start();

        update_tick = SDL_GetTicks();

        SDL_TimerID update_timer_id = SDL_AddTimer(
            1000 / configuration.update_freq, update_timer_callback, this);

        if (update_timer_id == 0)
        {
            *oerr << "Failed to create update timer. Error: " << SDL_GetError();
            return return_code::create_timer_fail;
        }

        SDL_TimerID render_timer_id = SDL_AddTimer(
            1000 / configuration.render_freq, render_timer_callback, this);

        if (render_timer_id == 0)
        {
            *oerr << "Failed to create update timer. Error: " << SDL_GetError();
            return return_code::create_timer_fail;
        }

        while (continue_loop)
        {
            while (SDL_PollEvent(&sdl_event))
            {
                event event;
                switch (sdl_event.type)
                {
                    case SDL_EVENT_KEY_DOWN:
                        event.type = event_type::key_pressed;
                        event.key  = get_key_name(sdl_event.key.keysym.sym);
                        break;
                    case SDL_EVENT_KEY_UP:
                        event.type = event_type::key_released;
                        event.key  = get_key_name(sdl_event.key.keysym.sym);
                        break;
                    case SDL_EVENT_QUIT:
                        event.type    = event_type::quit;
                        continue_loop = false;
                        break;
                    default:
                        event.type = event_type::unknown;
                }

                e_game->on_event(event);
            }
        }

        SDL_RemoveTimer(update_timer_id);
        SDL_RemoveTimer(render_timer_id);

        return return_code::good;
    };

    return_code render_fill_rect(const rect* rect) const override
    {
        if (SDL_RenderFillRect(renderer,
                               reinterpret_cast<const SDL_FRect*>(rect)) < 0)
        {
            *oerr << "Error to draw rect. Error: " << SDL_GetError();
            return return_code::render_fill_rect_fail;
        }

        return return_code::good;
    };

    return_code set_render_draw_color(unsigned char r,
                                      unsigned char g,
                                      unsigned char b,
                                      unsigned char a) override
    {
        if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0)
        {
            *oerr << "Failed to set draw color. Error: " << SDL_GetError();
            return return_code::set_render_draw_color_fail;
        }

        return return_code::good;
    };

    void update()
    {
        int curr_tick                = SDL_GetTicks();
        e_game->update_delta_time_ms = curr_tick - update_tick;

        e_game->update_time_ms = update_tick = curr_tick;

        e_game->on_update();
    };

    void render()
    {
        e_game->on_render();
        SDL_RenderPresent(renderer);
    };

private:
    game*         e_game   = nullptr;
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::ostream* oerr     = nullptr;

protected:
    key_name get_key_name(SDL_Keycode sdl_key_code)
    {
        switch (sdl_key_code)
        {
            case SDLK_LEFT:
                return key_name::left;
            case SDLK_RIGHT:
                return key_name::right;
            case SDLK_UP:
                return key_name::up;
            case SDLK_DOWN:
                return key_name::down;
            default:
                return key_name::unknown;
        }
    }

    int update_tick = 0;
    int render_tick = 0;
};

Uint32 update_timer_callback(Uint32 interval, void* param)
{
    engine_impl* engine = reinterpret_cast<engine_impl*>(param);
    engine->update();
    return interval;
}

Uint32 render_timer_callback(Uint32 interval, void* param)
{
    engine_impl* engine = reinterpret_cast<engine_impl*>(param);
    engine->render();
    return interval;
}

engine* engine_impl::instance = nullptr;

engine::~engine() = default;

engine* get_engine_instance()
{
    if (engine_impl::instance == nullptr)
        engine_impl::instance = new engine_impl();

    return engine_impl::instance;
}

}; // namespace Kengine
