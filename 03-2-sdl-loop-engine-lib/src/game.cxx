#include "engine.hxx"
#include <iostream>
#include <math.h>
#include <vector>

using namespace Kengine;

const int rects_amount = 10;

class my_game : public game
{
public:
    my_game(engine* game_engine)
        : game("My game", game_engine){};

    void on_start() override
    {
        int step = 640 / rects_amount;
        for (int i = 0; i < rects_amount; i++)
        {
            rects[i].x = step * (i + 0.25f);
            rects[i].y = 240;
            rects[i].w = step / 2.0f;
            rects[i].h = 240;
        }
    };

    void on_event(event e) override{

    };

    void on_update() override
    {
        for (int i = 0; i < rects_amount; i++)
        {
            float x      = rects[i].x;
            float height = 200 * (1 + sin(update_time_ms / 1000.0f + x) *
                                          cos(update_time_ms / 1000.0f + x));
            rects[i].y   = 480 - height;
            rects[i].h   = height;
        }
    };

    void on_render() const override
    {
        game_engine->set_render_draw_color(0, 0, 0, 255);
        game_engine->render_fill_rect(nullptr);

        for (const rect& r : rects)
        {
            game_engine->set_render_draw_color(
                155, 255 * sin(r.h / 200), 255 * cos(r.h / 200), 255);
            game_engine->render_fill_rect(&r);
        }
    };

private:
    std::vector<rect> rects = std::vector<rect>(rects_amount);
};

int main()
{
    engine* engine   = get_engine_instance();
    game*   new_game = new my_game(engine);

    if (engine->initialize(new_game, std::cerr) != return_code::good)
        return EXIT_FAILURE;

    engine->start_game_loop();

    engine->uninitialize();

    return EXIT_SUCCESS;
};
