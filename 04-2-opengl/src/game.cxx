#include "engine.hxx"
#include "shader-program.hxx"
#include "vertex-array-object.hxx"
#include "vertex.hxx"

#include <ctime>
#include <iostream>
#include <math.h>
#include <ratio>
#include <vector>

using namespace Kengine;

high_resolution_clock game_clock;

class my_game : public game
{
public:
    my_game(engine* game_engine)
        : game("My game", game_engine){};

    high_resolution_clock::time_point start_point;

    void on_start() override
    {
        vao         = create_vertex_array_object(vertices);
        program     = create_shader_program("./vertex.glsl", "./fragment.glsl");
        start_point = game_clock.now();
    };

    void on_event(event e) override{};

    virtual void on_update(duration<int, std::milli> delta_time) override{};

    virtual void on_render(duration<int, std::milli> delta_time) const override
    {
        game_engine->clear_color({ 0.0, 0.0, 0.0, 0.0 });
        program->use();
        float time = (game_clock.now() - start_point).count() / 1'000'000'000.f;
        program->set_uniform1f("time", time);
        vao->draw_triangles(1);
        game_engine->swap_buffers();
    };

    ~my_game() override
    {
        delete program;
        // delete vao;
    };

private:
    shader_program*      program = nullptr;
    vertex_array_object* vao     = nullptr;
    vertex_array vertices{ { { -0.5, 0.5, 0.0 }, { 1.0, 0.0, 0.0, 0.0 } },
                           { { 0.5, 0.5, 0.0 }, { 0.0, 1.0, 0.0, 0.0 } },
                           { { 0.0, -0.5, 0.0 }, { 0.0, 0.0, 1.0, 0.0 } } };
};

game* create_game(engine* e)
{
    if (e != nullptr)
        return new my_game(e);

    return nullptr;
};
