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

    virtual void on_update(duration<int, std::milli> delta_time) override{};

    virtual void on_render(duration<int, std::milli> delta_time) const override
    {
        game_engine->clear_color({ 1.0, 0.0, 1.0, 0.0 });
        game_engine->swap_buffers();
    };

private:
    std::vector<rect> rects = std::vector<rect>(rects_amount);
};

game* create_game(engine* e)
{
    if (e != nullptr)
        return new my_game(e);

    return nullptr;
};
