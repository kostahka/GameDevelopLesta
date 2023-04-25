#include "04-render-conveyor.hxx"
#include <cmath>

position interpolate(const position& p1, const position& p2, const double t)
{
    return { static_cast<int32_t>(std::round(t * p2.x + (1 - t) * p1.x)),
             static_cast<int32_t>(std::round(t * p2.y + (1 - t) * p1.y)) };
};

position_3d interpolate(const position_3d& p1,
                        const position_3d& p2,
                        const double       t)
{
    return { p2.x * t + (1 - t) * p1.x,
             p2.y * t + (1 - t) * p1.y,
             p2.z * t + (1 - t) * p1.z };
};

color interpolate(const color& c1, const color& c2, const double t)
{
    return { static_cast<uint8_t>(std::round(c2.r * t + (1 - t) * c1.r)),
             static_cast<uint8_t>(std::round(c2.g * t + (1 - t) * c1.g)),
             static_cast<uint8_t>(std::round(c2.b * t + (1 - t) * c1.b)) };
};

vertex interpolate(const vertex& v1, const vertex& v2, const double t)
{
    return { interpolate(v1.pos, v2.pos, t), interpolate(v1.col, v2.col, t) };
};

pixel interpolate(const pixel& p1, const pixel& p2, const double t)
{
    return { interpolate(p1.col, p2.col, t), interpolate(p1.pos, p2.pos, t) };
};

render_conveyor::render_conveyor(canvas*           canv,
                                 ivertex_shader*   v_shader,
                                 ifragment_shader* f_shader)
    : main_renderer(canv)
    , v_shader(v_shader)
    , f_shader(f_shader)
{
    w = canv->get_width();
    h = canv->get_height();
    secondary_buffer.resize(w * h);
};

void render_conveyor::clear(color col)
{
    std::fill(secondary_buffer.begin(), secondary_buffer.end(), col);
};

void render_conveyor::switch_buffers()
{
    secondary_buffer.swap(main_renderer->get_buffer());
};

void render_conveyor::rasterize_horizontal_line(pixel         left,
                                                pixel         right,
                                                pixel_vector& out)
{
    for (int x = left.pos.x; x <= right.pos.x; x++)
    {
        double t         = static_cast<double>(x - left.pos.x) / right.pos.x;
        pixel  new_pixel = interpolate(left, right, t);
        out.push_back(new_pixel);
    }
};

void render_conveyor::set_pixel(pixel pixel)
{
    const int32_t index     = w * pixel.pos.y + pixel.pos.x;
    secondary_buffer[index] = pixel.col;
};

void render_conveyor::draw_pixels(pixel_vector& pixels)
{
    for (pixel& p : pixels)
    {
        if (p.pos.x >= 0 && p.pos.x < w && p.pos.y >= 0 && p.pos.y < h)
        {
            set_pixel(p);
        }
    }
};

void render_conveyor::rasterize_horizontal_triangle(pixel         left,
                                                    pixel         right,
                                                    pixel         v3,
                                                    pixel_vector& out)
{
    int32_t y_step  = v3.pos.y < left.pos.y ? 1 : -1;
    int32_t y_delta = std::abs(v3.pos.y - left.pos.y);
    for (int32_t y = v3.pos.y; y != left.pos.y; y += y_step)
    {
        double t = static_cast<double>(std::abs(y - v3.pos.y)) / y_delta;
        pixel  line_left  = interpolate(left, v3, t);
        pixel  line_right = interpolate(right, v3, t);
        rasterize_horizontal_line(line_left, line_right, out);
    }
};

void render_conveyor::rasterize_triangle(pixel         v1,
                                         pixel         v2,
                                         pixel         v3,
                                         pixel_vector& out)
{
    pixel middle;
    pixel _v1;
    pixel _v2;

    auto is_between = [](pixel& v, pixel& v2, pixel& v3)
    {
        return (v.pos.y < v2.pos.y && v.pos.y > v3.pos.y) ||
               (v.pos.y > v2.pos.y && v.pos.y < v3.pos.y);
    };

    if (is_between(v1, v2, v3))
    {
        middle = v1;
        _v1    = v2;
        _v2    = v3;
    }
    else if (is_between(v2, v1, v3))
    {
        middle = v2;
        _v1    = v1;
        _v2    = v3;
    }
    else
    {
        middle = v3;
        _v1    = v1;
        _v2    = v2;
    }

    double horizontal_t =
        static_cast<double>(std::abs(middle.pos.y - _v1.pos.y)) /
        std::abs(_v1.pos.y - _v2.pos.y);

    pixel horizontal_v = interpolate(_v1, _v2, horizontal_t);

    auto is_left = [](pixel& v, pixel v2) { return v.pos.x < v2.pos.x; };

    if (is_left(middle, horizontal_v))
    {
        rasterize_horizontal_triangle(middle, horizontal_v, _v1, out);
        rasterize_horizontal_triangle(middle, horizontal_v, _v2, out);
    }
    else
    {
        rasterize_horizontal_triangle(horizontal_v, middle, _v1, out);
        rasterize_horizontal_triangle(horizontal_v, middle, _v2, out);
    }
};

void render_conveyor::draw_triangle(vertex v1, vertex v2, vertex v3)
{
    v_shader->modify(v1);
    v_shader->modify(v2);
    v_shader->modify(v3);

    pixel_vector pixels;

    auto to_pixel = [](vertex& v)
    {
        return pixel{ v.col,
                      { static_cast<int32_t>(std::round(v.pos.x)),
                        static_cast<int32_t>(std::round(v.pos.y)) } };
    };

    rasterize_triangle(to_pixel(v1), to_pixel(v2), to_pixel(v3), pixels);

    for (pixel& p : pixels)
    {
        f_shader->modify(p);
    }
    draw_pixels(pixels);
};

void render_conveyor::draw_triangles(vertex_vector& vertices,
                                     index_vector&  indexes)
{
    for (size_t i = 0; i < indexes.size() / 3; i++)
    {
        size_t index1 = indexes.at(i * 3 + 0);
        size_t index2 = indexes.at(i * 3 + 1);
        size_t index3 = indexes.at(i * 3 + 2);

        vertex& v1 = vertices.at(index1);
        vertex& v2 = vertices.at(index2);
        vertex& v3 = vertices.at(index3);

        draw_triangle(v1, v2, v3);
    }
};
