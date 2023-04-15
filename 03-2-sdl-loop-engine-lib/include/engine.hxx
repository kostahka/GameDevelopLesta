#ifndef ENGINE_H_
#define ENGINE_H_

#include <ostream>
#include <string>

namespace Kengine
{

class game;
class engine;

struct rect
{
    float x;
    float y;
    float w;
    float h;
};

enum class return_code
{
    good                       = 0,
    sdl_init_fail              = 1,
    create_window_fail         = 2,
    create_renderer_fail       = 3,
    game_not_set               = 4,
    render_fill_rect_fail      = 5,
    set_render_draw_color_fail = 6,
    create_timer_fail          = 7,
};

enum class event_type
{
    key_pressed,
    key_released,

    quit,

    unknown
};

enum class key_name
{
    left,
    right,
    up,
    down,

    unknown
};

struct event
{
    event_type type;
    union
    {
        key_name key;
    };
};

// game configuratiions
struct game_configuration
{
    // window sizes
    int screen_width;
    int screen_height;
};

// Engine configuration
struct engine_configuration
{
    // frequency of physics update
    int update_freq;
    // frequency of render update
    int render_freq;
};

class engine
{
public:
    virtual ~engine();
    virtual return_code initialize(game*, std::ostream& error_stream) = 0;
    virtual return_code uninitialize()                                = 0;

    virtual return_code start_game_loop() = 0;

    virtual return_code render_fill_rect(const rect*) const    = 0;
    virtual return_code set_render_draw_color(unsigned char r,
                                              unsigned char g,
                                              unsigned char b,
                                              unsigned char a) = 0;

    engine_configuration configuration{ 60, 90 };
};

engine* get_engine_instance();

class game
{
public:
    game(std::string name, engine* engine)
        : name(name)
        , game_engine(engine){};

    virtual ~game()                = default;
    virtual void on_start()        = 0;
    virtual void on_event(event)   = 0;
    virtual void on_update()       = 0;
    virtual void on_render() const = 0;

    int                update_delta_time_ms = 0;
    int                update_time_ms       = 0;
    std::string        name                 = "";
    engine*            game_engine          = nullptr;
    game_configuration configuration{ 640, 480 };
};

}; // namespace Kengine

#endif // ENGINE_H_
