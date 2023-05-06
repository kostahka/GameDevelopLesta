#include "engine.hxx"

#include <SDL3/SDL.h>
#include <chrono>
#include <efsw/efsw.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <ratio>
#include <string>
#include <string_view>

namespace Kengine
{

class engine_impl;

enum class user_events
{
#ifdef ENGINE_DEV
    reload_game = 0,
#endif
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

class engine_impl : public engine
{
public:
    static engine* instance;

    std::string_view initialize() override
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Error to initialize SDL. Error: " << SDL_GetError()
                      << std::endl;
            return "sdl init fail";
        }

        return "good";
    };

    std::string_view uninitialize() override
    {
        if (window)
            SDL_DestroyWindow(window);

        SDL_Quit();

        return "good";
    };

    std::string_view start_game_loop() override
    {
        if (e_game == nullptr)
            return "game not set";

        window = SDL_CreateWindow(e_game->name.c_str(),
                                  e_game->configuration.screen_width,
                                  e_game->configuration.screen_height,
                                  SDL_WINDOW_OPENGL);

        if (window == nullptr)
        {
            std::cerr << "Error to create window. Error: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
            return "failed to create window";
        }

        bool      continue_loop = true;
        SDL_Event sdl_event;

        e_game->on_start();

        update_time = game_clock.now();
        render_time = game_clock.now();

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
#ifdef ENGINE_DEV
                            case (int)user_events::reload_game:
                                reload_game();
                                break;
#endif
                            default:
                                break;
                        }
                    default:
                        event.type = event_type::unknown;
                }

                e_game->on_event(event);
            }

            auto update_delta_time = game_clock.now() - update_time;
            if (update_delta_time > configuration.update_delta_time)
            {
                e_game->on_update(
                    duration_cast<milliseconds>(update_delta_time));
                update_time = game_clock.now();
            }
            auto render_delta_time = game_clock.now() - render_time;
            if (render_delta_time > configuration.render_delta_time)
            {
                e_game->on_render(
                    duration_cast<milliseconds>(render_delta_time));
                render_time = game_clock.now();
            }
        }

        return "good";
    };

    void set_game(game* e_game) override { this->e_game = e_game; }

#ifdef ENGINE_DEV
    std::string_view dev_initialization(std::string lib_name,
                                        std::string tmp_lib_name) override
    {
        this->lib_name     = lib_name;
        this->tmp_lib_name = tmp_lib_name;
        return "good";
    };

    std::string_view start_dev_game_loop() override
    {
        if (lib_name == "" || tmp_lib_name == "")
            return "no dev init";

        reload_game();

        efsw::FileWatcher game_file_watcher;

        efsw::WatchID game_file_watch_id = game_file_watcher.addWatch(
            lib_name, &game_file_update_listener, false);

        game_file_watcher.watch();
        auto loop_return_code = start_game_loop();
        game_file_watcher.removeWatch(game_file_watch_id);

        return loop_return_code;
    };

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
                std::cerr << "Failed to remove temp lib [" << tmp_lib_name
                          << "]" << std::endl;
                return false;
            }
        }

        try
        {
            copy_file(lib_name, tmp_lib_name);
        }
        catch (const std::exception* ex)
        {
            std::cerr << "Failed to copy from [" << lib_name << "] to ["
                      << tmp_lib_name << "]" << std::endl;
            return false;
        }

        lib_handle = SDL_LoadObject(tmp_lib_name.c_str());

        if (lib_handle == nullptr)
        {
            std::cerr << "Failed to load lib from [" << tmp_lib_name << "]"
                      << std::endl;
            return false;
        }

        SDL_FunctionPointer create_game_func_ptr =
            SDL_LoadFunction(lib_handle, "create_game");

        if (create_game_func_ptr == nullptr)
        {
            std::cerr << "Failed to load function [create_game] from ["
                      << tmp_lib_name << "]" << std::endl;
            return false;
        }

        using create_game_ptr = decltype(&create_game);

        create_game_ptr create_game_func =
            reinterpret_cast<create_game_ptr>(create_game_func_ptr);

        game* new_game = create_game_func(this);

        if (new_game == nullptr)
        {
            std::cerr << "Failed to create game" << std::endl;
            return false;
        }

        e_game = new_game;

        e_game->on_start();

        return true;
    };

    std::string lib_name     = "";
    std::string tmp_lib_name = "";
    void*       lib_handle   = nullptr;

    class GameFileUpdate : public efsw::FileWatchListener
    {
    public:
        void handleFileAction(efsw::WatchID      watchid,
                              const std::string& dir,
                              const std::string& filename,
                              efsw::Action       action,
                              std::string        oldFilename) override
        {
            switch (action)
            {
                case efsw::Actions::Modified:
                    push_user_event(static_cast<int>(user_events::reload_game));
                    break;
                case efsw::Actions::Add:
                    break;
                case efsw::Actions::Delete:
                    break;
                case efsw::Actions::Moved:
                    break;
                default:
                    break;
            }
        };
    } game_file_update_listener;
#endif

private:
    // Engine
    game*       e_game = nullptr;
    SDL_Window* window = nullptr;
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
    high_resolution_clock::time_point update_time;
    high_resolution_clock::time_point render_time;

    high_resolution_clock game_clock;
};

engine* engine_impl::instance = nullptr;

engine::~engine() = default;

engine* get_engine_instance()
{
    if (engine_impl::instance == nullptr)
        engine_impl::instance = new engine_impl();

    return engine_impl::instance;
}
}; // namespace Kengine

#ifdef ENGINE_DEV
int main()
{
    using namespace Kengine;
    engine* engine = get_engine_instance();

    if (engine->initialize() != "good")
        return EXIT_FAILURE;

    using namespace std::string_literals;
    std::string game_name = ENGINE_GAME_LIB_NAME;
    std::string lib_name  = SDL_GetPlatform() == "Windows"s
                                ? game_name + ".dll"
                                : "./lib" + game_name + ".so";

    std::string tmp_lib_name = "./temp.dll";

    engine->dev_initialization(lib_name, tmp_lib_name);

    engine->start_dev_game_loop();

    engine->uninitialize();

    return EXIT_SUCCESS;
};
#endif
