#include "engine.hxx"

#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader-program.hxx"
#include "texture.hxx"
#include "transform3d.hxx"
#include "vertex-array-object.hxx"
#include "vertex.hxx"

#include <ctime>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <iostream>
#include <math.h>
#include <numbers>
#include <ratio>
#include <vector>

using namespace Kengine;

high_resolution_clock game_clock;

constexpr float circle_radius         = 0.5;
constexpr int   circle_segments_count = 20;

class my_game : public game
{
public:
    my_game(engine* game_engine)
        : game("My game", game_engine){};

    high_resolution_clock::time_point start_point;

    void on_start() override
    {
        float delta_angle = 2 * std::numbers::pi / circle_segments_count;
        for (int i = 0; i < circle_segments_count; i++)
        {
            float x1 = std::sin(delta_angle * i) * circle_radius;
            float y1 = std::cos(delta_angle * i) * circle_radius;
            float x2 = std::sin(delta_angle * (i + 1)) * circle_radius;
            float y2 = std::cos(delta_angle * (i + 1)) * circle_radius;

            if (i == 0)
                y1 += 0.2;
            if (i == circle_segments_count - 1)
                y2 += 0.2;

            vertices.push_back({ { 0.0, 0.0, 0.0 }, { 0.0, 0.0 } });
            vertices.push_back({ { x1, y1, 0.0 }, { x1, y1 } });
            vertices.push_back({ { x2, y2, 0.0 }, { x2, y2 } });
        }

        lava_texture = create_texture("./assets/lava.png");
        vao          = create_static_vao_text2d(vertices);
        program     = create_shader_program("./vertex.glsl", "./fragment.glsl");
        start_point = game_clock.now();
    };

    void on_event(event e) override{};

    virtual void on_update(duration<int, std::milli> delta_time) override
    {
        if (is_key_pressed(key_name::left))
        {
            circle_angle -= circle_angle_speed * delta_time.count();
        }
        else if (is_key_pressed(key_name::right))
        {
            circle_angle += circle_angle_speed * delta_time.count();
        }

        if (is_key_pressed(key_name::down))
        {
            float delta = circle_y_speed * delta_time.count();
            circle_pos.x -= delta * std::sin(circle_angle);
            circle_pos.y -= delta * std::cos(circle_angle);
        }
        else if (is_key_pressed(key_name::up))
        {
            float delta = circle_y_speed * delta_time.count();
            circle_pos.x += delta * std::sin(circle_angle);
            circle_pos.y += delta * std::cos(circle_angle);
        }
    };

    virtual void on_render(duration<int, std::milli> delta_time) const override
    {
        game_engine->clear_color({ 0.0, 0.0, 0.0, 0.0 });
        lava_texture->bind();
        program->use();

        glm::mat3 transform_matrix(1.0f);
        transform_matrix = glm::translate(
            transform_matrix, glm::vec2(circle_pos.x, circle_pos.y));
        transform_matrix = glm::rotate(transform_matrix, -circle_angle);

        program->set_uniform_matrix3fv("v_trans_matrix", transform_matrix);

        float time = (game_clock.now() - start_point).count() / 1'000'000'000.f;
        program->set_uniform1f("time", time);

        vao->draw_triangles(circle_segments_count);
        game_engine->swap_buffers();
    };

    ~my_game() override
    {
        delete program;
        // delete vao;
    };

private:
    shader_program*             program = nullptr;
    static_vertex_array_object* vao     = nullptr;
    vertex_text2d_array         vertices;
    std::vector<transform3d>    mouse_vertices;
    texture_object*             lava_texture       = nullptr;
    float                       circle_angle       = 0;
    float                       circle_angle_speed = 0.001;
    transform2d                 circle_pos         = { 0.0, 0.0 };
    float                       circle_y_speed     = 0.001;
};

game* create_game(engine* e)
{
    if (e != nullptr)
        return new my_game(e);

    return nullptr;
};
