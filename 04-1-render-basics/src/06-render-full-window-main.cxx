#include <cmath>
#include <iostream>
#include <numbers>

#include <SDL3/SDL.h>

#include "04-render-conveyor.hxx"
#include "PerlinNoise.hpp"

const int screen_width  = 320;
const int screen_height = 280;

constexpr color color_black{ 0, 0, 0 };

//  <------------ Sun characteristics ------------------>
constexpr double sun_radius         = 50.0;
constexpr double sun_radius_delta   = 5.0;
constexpr int    sun_vertices_count = 20;
constexpr int    sun_indexes_count  = sun_vertices_count * 3;

constexpr color sun_center_color{ 240, 150, 15 };
constexpr color sun_end_color{ 255, 191, 0 };

constexpr double sun_delta_angle = 2.0 * std::numbers::pi / sun_vertices_count;
constexpr double sun_delta_reflection = 20.0;

constexpr position_3d sun_center{ screen_width / 2.0, screen_height / 2.0, 0 };

//  <--- Background(sea and sky) characteristics ------->

//  <------------ Sea characteristics ------------------>
constexpr int   sea_level = screen_height / 2;
constexpr color sea_color_top{ 0, 102, 204 };
constexpr color sea_color_bottom{ 0, 76, 153 };
constexpr color sea_color_wave{ 40, 60, 100 };

//  <------------ Sky characteristics ------------------>
constexpr color sky_color_top{ 206, 226, 77 };
constexpr color sky_color_bottom{ 253, 41, 69 };
constexpr color sky_color_cloud{ 253, 240, 230 };

constexpr int sea_level_delta = 7;

//  <------------ Perlin noise init -------------------->
const siv::PerlinNoise::seed_type seed = 1234;
const siv::PerlinNoise            perlin{ seed };
// Perlin noise - procedural texture primitive

// <------------- Sun render program ------------------->
struct sun_render_program : public irender_program
{
    double time    = 0;
    double mouse_x = 0;
    double mouse_y = 0;
    double radius  = 20.0;
    void   set_uniforms(const uniforms& u)
    {
        time    = u.f1;
        mouse_x = u.f2;
        mouse_y = u.f3;
        radius  = u.f4;
    };
    vertex vertex_shader(const vertex& v)
    {
        // Sun waves simulation
        double delta_x =
            sun_radius_delta * std::cos(time) * std::sin(time + v.pos.x);
        double delta_y =
            sun_radius_delta * std::sin(time) * std::sin(time + v.pos.x);

        // Stretching sun reflection
        if (v.pos.y > sea_level)
        {
            delta_x += (-(v.pos.x - sun_center.x) / sun_radius) *
                       (v.pos.y - sea_level);
            delta_y = -delta_y + sea_level - 2 * sun_radius;
        }

        position_3d delta_pos{ delta_x, delta_y, 0 };

        return { v.pos + delta_pos, v.col };
    };
    color fragment_shader(const pixel& p)
    {
        // Simulation of sea waves borders
        double noise       = perlin.noise2D_01(p.pos.x / 50.0, time / 10.0);
        int    delta_level = std::round(sea_level_delta * noise);

        // Sun reflection in sea
        if (p.pos.y >= sea_level - delta_level)
            return interpolate(sea_color_top, p.col, noise / 2);
        return p.col;
    };
};

// <-------------- Background render program ------------------>
struct background_render_program : public irender_program
{
    double time    = 0;
    double mouse_x = 0;
    double mouse_y = 0;
    double radius  = 20.0;
    void   set_uniforms(const uniforms& u)
    {
        time    = u.f1;
        mouse_x = u.f2;
        mouse_y = u.f3;
        radius  = u.f4;
    };
    vertex vertex_shader(const vertex& v) { return v; };
    color  fragment_shader(const pixel& p)
    {
        // Sea border waves noise
        double noise       = perlin.noise2D_01(p.pos.x / 50.0, time / 10.0);
        int    delta_level = std::round(sea_level_delta * noise);

        color col = p.col;

        // Sea and sky border
        if (p.pos.y < sea_level && p.pos.y + delta_level > sea_level)
            col = sea_color_top;
        else if (p.pos.y >= sea_level && p.pos.y + delta_level < sea_level)
            col = sky_color_bottom;

        // Waves or clouds filling noise
        noise = perlin.noise3D_01(p.pos.x / 50.0, p.pos.y / 10.0, time / 10.0);

        // Waves or clouds filling
        if (p.pos.y >= sea_level - delta_level)
        {
            return interpolate(col, sea_color_wave, noise);
        }
        else
            return interpolate(col, sky_color_cloud, noise / 5);
    };
};

int main()
{
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Failed to initialize sdl video error. Error: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Sunset", screen_width, screen_height, 0);
    if (window == nullptr)
    {
        std::cerr << "Failed to create window. Error: " << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, "opengl", SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Failed to create renderer. Error: " << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }

    canvas                    image(screen_width, screen_height);
    render_conveyor           my_renderer(&image);
    sun_render_program        sun_program;
    background_render_program background_program;

    // Sun vertices
    //        --*--
    //      *       *
    //    /           \
    //   *      *      *
    //    \           /
    //      *       *
    //        --*--
    // 1 in centere, other on border
    //
    vertex_vector sun_vertices;

    sun_vertices.reserve(sun_vertices_count + 1);
    sun_vertices.push_back({ sun_center, sun_center_color });
    for (int i = 0; i < sun_vertices_count; i++)
    {
        double angle = i * sun_delta_angle;
        sun_vertices.push_back({ { sun_center.x + sun_radius * std::cos(angle),
                                   sun_center.y + sun_radius * std::sin(angle),
                                   0 },
                                 sun_end_color });
    }

    index_vector sun_indexes;
    sun_indexes.reserve(sun_indexes_count);
    uint16_t index0 = 0;
    for (int i = 0; i < sun_vertices_count; i++)
    {
        uint16_t index1 = (i % (sun_vertices_count)) + 1;
        uint16_t index2 = ((i + 1) % (sun_vertices_count)) + 1;

        sun_indexes.push_back(index0);
        sun_indexes.push_back(index1);
        sun_indexes.push_back(index2);
    }

    // Background vertices
    //
    //   *---------------------*
    //   |                     |
    //   |         sky         |
    //   |                     |
    //   *---------------------*
    //   *---------------------*
    //   |                     |
    //   |         sea         |
    //   |                     |
    //   *---------------------*
    //  8 vertices
    //
    vertex_vector background_vertices;
    // clang-format off
    background_vertices.push_back(
        { { 0, 0, 0 }, sky_color_top });
    background_vertices.push_back(
        { { screen_width - 1, 0, 0 }, sky_color_top });
    background_vertices.push_back(
        { { screen_width - 1, sea_level, 0 }, sky_color_bottom });
    background_vertices.push_back(
        { { 0, sea_level, 0 }, sky_color_bottom });

    background_vertices.push_back(
        { { 0, sea_level, 0 }, sea_color_top });
    background_vertices.push_back(
        { { screen_width-1, sea_level, 0 }, sea_color_top });
    background_vertices.push_back(
        { { screen_width-1, screen_height-1, 0 }, sea_color_bottom });
    background_vertices.push_back(
        { { 0, screen_height-1, 0 }, sea_color_bottom });
    // clang-format on
    index_vector background_indexes;
    background_indexes.push_back(0);
    background_indexes.push_back(1);
    background_indexes.push_back(2);
    background_indexes.push_back(2);
    background_indexes.push_back(3);
    background_indexes.push_back(0);

    background_indexes.push_back(4);
    background_indexes.push_back(5);
    background_indexes.push_back(6);
    background_indexes.push_back(6);
    background_indexes.push_back(7);
    background_indexes.push_back(4);

    void*     pixels = image.get_buffer().data();
    const int pitch  = sizeof(color) * screen_width;

    double time{ 0 };
    double mouse_x{ 100 };
    double mouse_y{ 100 };
    double radius{ 20 };

    SDL_Event e;
    bool      quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_EVENT_MOUSE_MOTION)
            {
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
            }
            else if (e.type == SDL_EVENT_MOUSE_WHEEL)
            {
                radius += e.wheel.y;
            }
        }

        time = SDL_GetTicks() / 500.0;
        uniforms u{ time, mouse_x, mouse_y, radius };
        my_renderer.clear(color_black);

        my_renderer.set_program(&background_program);
        background_program.set_uniforms(u);
        my_renderer.draw_triangles(background_vertices, background_indexes);

        my_renderer.set_program(&sun_program);
        sun_program.set_uniforms(u);
        my_renderer.draw_triangles(sun_vertices, sun_indexes);

        my_renderer.switch_buffers();

        SDL_Surface* bitmap_surface = SDL_CreateSurfaceFrom(
            pixels, screen_width, screen_height, pitch, SDL_PIXELFORMAT_RGB24);

        if (bitmap_surface == nullptr)
        {
            std::cerr << "Failed to create bitmap surface. Error: "
                      << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }
        SDL_Texture* bitmap_texture =
            SDL_CreateTextureFromSurface(renderer, bitmap_surface);
        if (bitmap_texture == nullptr)
        {
            std::cerr << "Failed to create bitmap texture. Error: "
                      << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }
        SDL_DestroySurface(bitmap_surface);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, bitmap_texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(bitmap_texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
