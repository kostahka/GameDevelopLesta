#include "engine.hxx"
#include <SDL3/SDL.h>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

namespace Kengine
{

enum class user_events
{
    update_event = 1,
    render_event = 2,
    reload_game  = 3,
};

void push_user_event(int user_event_code)
{
    SDL_Event     event;
    SDL_UserEvent user_event;

    user_event.code = user_event_code;

    user_event.type = SDL_EVENT_USER;

    event.type = SDL_EVENT_USER;
    event.user = user_event;

    SDL_PushEvent(&event);
};
class engine_impl;
Uint32 update_timer_callback(Uint32 interval, void* param);
Uint32 render_timer_callback(Uint32 interval, void* param);
Uint32 hot_reload_timer_callback(Uint32 interval, void* param);

class engine_impl : public engine
{
public:
    static engine* instance;

    return_code initialize(std::ostream& error_stream) override
    {
        this->oerr = &error_stream;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            *oerr << "Error to initialize SDL. Error: " << SDL_GetError()
                  << std::endl;
            return return_code::sdl_init_fail;
        }

        return return_code::good;
    };

    return_code dev_initialization(std::string lib_name,
                                   std::string tmp_lib_name) override
    {
        this->lib_name     = lib_name;
        this->tmp_lib_name = tmp_lib_name;
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

        window = SDL_CreateWindow(e_game->name.c_str(),
                                  e_game->configuration.screen_width,
                                  e_game->configuration.screen_height,
                                  SDL_WINDOW_OPENGL);

        if (window == nullptr)
        {
            *oerr << "Error to create window. Error: " << SDL_GetError()
                  << std::endl;
            SDL_Quit();
            return return_code::create_window_fail;
        }

        renderer = SDL_CreateRenderer(window, "opengl", SDL_RENDERER_SOFTWARE);

        if (renderer == nullptr)
        {
            *oerr << "Failed to create a renderer. Error: " << SDL_GetError()
                  << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return return_code::create_renderer_fail;
        }

        update_tick = SDL_GetTicks();

        SDL_TimerID update_timer_id = SDL_AddTimer(
            1000 / configuration.update_freq, update_timer_callback, this);

        if (update_timer_id == 0)
        {
            *oerr << "Failed to create update timer. Error: " << SDL_GetError()
                  << std::endl;
            return return_code::create_timer_fail;
        }

        SDL_TimerID render_timer_id = SDL_AddTimer(
            1000 / configuration.render_freq, render_timer_callback, this);

        if (render_timer_id == 0)
        {
            *oerr << "Failed to create update timer. Error: " << SDL_GetError()
                  << std::endl;
            return return_code::create_timer_fail;
        }

        bool      continue_loop = true;
        SDL_Event sdl_event;

        e_game->on_start();

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
                    case SDL_EVENT_USER:
                        event.type = event_type::unknown;
                        switch (sdl_event.user.code)
                        {
                            case (int)user_events::reload_game:
                                reload_game();
                                break;
                            case (int)user_events::render_event:
                                render();
                                break;
                            case (int)user_events::update_event:
                                update();
                                break;
                            default:
                                break;
                        }
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

    return_code start_dev_game_loop() override
    {
        if (lib_name == "" || tmp_lib_name == "")
            return return_code::no_dev_init;

        reload_game();

        SDL_TimerID hot_reload_timer_id =
            SDL_AddTimer(200, hot_reload_timer_callback, this);

        if (hot_reload_timer_id == 0)
        {
            *oerr << "Failed to create hot reload timer. Error: "
                  << SDL_GetError() << std::endl;
            return return_code::create_timer_fail;
        }

        auto loop_return_code = start_game_loop();

        SDL_RemoveTimer(hot_reload_timer_id);

        return loop_return_code;
    };

    return_code render_fill_rect(const rect* rect) const override
    {
        if (SDL_RenderFillRect(renderer,
                               reinterpret_cast<const SDL_FRect*>(rect)) < 0)
        {
            *oerr << "Error to draw rect. Error: " << SDL_GetError()
                  << std::endl;
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
            *oerr << "Failed to set draw color. Error: " << SDL_GetError()
                  << std::endl;
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

    void set_game(game* e_game) override { this->e_game = e_game; }

    // Dev
    bool reload_game()
    {
        if (e_game)
        {
            delete e_game;
            SDL_UnloadObject(lib_handle);
        }

        using namespace std::filesystem;
        if (exists(tmp_lib_name))
        {
            if (!remove(tmp_lib_name))
            {
                *oerr << "Failed to remove temp lib [" << tmp_lib_name << "]"
                      << std::endl;
                return false;
            }
        }

        try
        {
            copy_file(lib_name, tmp_lib_name);
        }
        catch (const std::exception* ex)
        {
            *oerr << "Failed to copy from [" << lib_name << "] to ["
                  << tmp_lib_name << "]" << std::endl;
            return false;
        }

        lib_handle = SDL_LoadObject(tmp_lib_name.c_str());

        if (lib_handle == nullptr)
        {
            *oerr << "Failed to load lib from [" << tmp_lib_name << "]"
                  << std::endl;
            return false;
        }

        SDL_FunctionPointer create_game_func_ptr =
            SDL_LoadFunction(lib_handle, "create_game");

        if (create_game_func_ptr == nullptr)
        {
            *oerr << "Failed to load function [create_game] from ["
                  << tmp_lib_name << "]" << std::endl;
            return false;
        }

        using create_game_ptr = decltype(&create_game);

        create_game_ptr create_game_func =
            reinterpret_cast<create_game_ptr>(create_game_func_ptr);

        game* new_game = create_game_func(this);

        if (new_game == nullptr)
        {
            *oerr << "Failed to create game" << std::endl;
            return false;
        }

        e_game = new_game;

        e_game->on_start();

        return true;
    };

    std::string lib_name     = "";
    std::string tmp_lib_name = "";
    void*       lib_handle   = nullptr;

private:
    // Engine
    game*         e_game   = nullptr;
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::ostream* oerr     = nullptr;

    // SDL keys to engine keys
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

    // Time from init SDL in miliseconds
    int update_tick = 0;
    int render_tick = 0;
};

Uint32 update_timer_callback(Uint32 interval, void* param)
{
    push_user_event((int)user_events::update_event);
    return interval;
}

Uint32 render_timer_callback(Uint32 interval, void* param)
{
    push_user_event((int)user_events::render_event);
    return interval;
}

Uint32 hot_reload_timer_callback(Uint32 interval, void* param)
{
    engine_impl* engine = reinterpret_cast<engine_impl*>(param);

    using namespace std::filesystem;

    static bool           file_is_changing = false;
    static file_time_type time_during_loading =
        last_write_time(engine->lib_name);

    static file_time_type current_write_time;
    static file_time_type next_write_time;

    if (file_is_changing)
    {
        next_write_time = last_write_time(engine->lib_name);

        if (next_write_time == current_write_time)
        {
            push_user_event((int)user_events::reload_game);
            file_is_changing    = false;
            time_during_loading = next_write_time;
            return 200;
        }
        else
        {
            current_write_time = next_write_time;
            return 1000;
        }
    }
    else
    {
        current_write_time = last_write_time(engine->lib_name);
        if (current_write_time != time_during_loading)
        {
            file_is_changing = true;
            return 1000;
        }
    }
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

int main()
{
    using namespace Kengine;
    engine* engine = get_engine_instance();

    if (engine->initialize(std::cerr) != return_code::good)
        return EXIT_FAILURE;

    using namespace std::string_literals;
    std::string lib_name = SDL_GetPlatform() == "Windows"s
                               ? "game-hot-load-shared.dll"
                               : "./libgame-hot-load-shared.so";

    std::string tmp_lib_name = "./temp.dll";

    engine->dev_initialization(lib_name, tmp_lib_name);

    engine->start_dev_game_loop();

    engine->uninitialize();

    return EXIT_SUCCESS;
};
