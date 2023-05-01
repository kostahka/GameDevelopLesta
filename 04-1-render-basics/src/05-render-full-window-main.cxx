#include <cmath>
#include <iostream>
#include <numbers>

#include <SDL3/SDL.h>

#include "04-render-conveyor.hxx"

const int screen_width  = 320;
const int screen_height = 280;

constexpr color color_black{ 0, 0, 0 };

constexpr double circle_radius         = 100.0;
constexpr double circle_radius_delta   = 50.0;
constexpr int    circle_vertices_count = 20;
constexpr int    circle_indexes_count  = circle_vertices_count * 3;

constexpr color circle_center_color{ 253, 218, 13 };
constexpr color circle_end_color{ 255, 191, 0 };

constexpr double circle_delta_angle =
    2.0 * std::numbers::pi / circle_vertices_count;

constexpr position_3d circle_center{ screen_width / 2.0,
                                     screen_height / 2.0,
                                     0 };

struct render_program : public irender_program
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
        double delta_x = circle_radius_delta * std::cos(time) *
                         std::sin(time + v.pos.x + v.pos.y);
        double delta_y = circle_radius_delta * std::sin(time) *
                         std::sin(time + v.pos.x + v.pos.y);

        position_3d delta_pos{ delta_x, delta_y, 0 };

        return { v.pos + delta_pos, v.col };
    };
    color fragment_shader(const pixel& p)
    {
        pixel p_mouse{ { 0, 0, 0 },
                       { static_cast<int32_t>(std::round(mouse_x)),
                         static_cast<int32_t>(std::round(mouse_y)) } };

        double distance = std::sqrt(std::pow(p_mouse.pos.x - p.pos.x, 2) +
                                    std::pow(p_mouse.pos.y - p.pos.y, 2));

        double new_radius = radius + 5.0 * sin(time + std::sin(p.pos.x)) *
                                         std::cos(time * cos(p.pos.y));
        if (distance < new_radius)
        {
            return { 0, 0, 0 };
        }
        else if (distance - new_radius < 0.5 * new_radius)
        {
            double t = (distance - new_radius) / (new_radius * 0.5);

            return interpolate({ 255, 255, 255 }, p.col, t);
        }
        else
        {
            return p.col;
        }
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

    window = SDL_CreateWindow("Hello SDL", screen_width, screen_height, 0);
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

    canvas          image(screen_width, screen_height);
    render_conveyor my_renderer(&image);
    render_program  program;

    my_renderer.set_program(&program);

    vertex_vector circle_vertices;

    circle_vertices.reserve(circle_vertices_count + 1);
    circle_vertices.push_back({ circle_center, circle_center_color });
    for (int i = 0; i < circle_vertices_count; i++)
    {
        double angle = i * circle_delta_angle;
        circle_vertices.push_back(
            { { circle_center.x + circle_radius * std::cos(angle),
                circle_center.y + circle_radius * std::sin(angle),
                0 },
              circle_end_color });
    }

    index_vector circle_indexes;
    circle_indexes.reserve(circle_indexes_count);
    uint16_t index0 = 0;
    for (int i = 0; i < circle_vertices_count; i++)
    {
        uint16_t index1 = (i % (circle_vertices_count)) + 1;
        uint16_t index2 = ((i + 1) % (circle_vertices_count)) + 1;

        circle_indexes.push_back(index0);
        circle_indexes.push_back(index1);
        circle_indexes.push_back(index2);
    }

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
        program.set_uniforms(u);

        my_renderer.clear(color_black);
        my_renderer.draw_triangles(circle_vertices, circle_indexes);

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
