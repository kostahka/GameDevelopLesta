#pragma once

#include "00-canvas-basic.hxx"

struct position_3d
{
    double x;
    double y;
    double z;
};

struct vertex
{
    position_3d pos;
    color       col{ 0, 0, 0 };
};

using vertex_vector = std::vector<vertex>;
using index_vector  = std::vector<uint16_t>;

class ifragment_shader
{
public:
    virtual void modify(pixel& pixel) = 0;
};

class ivertex_shader
{
public:
    virtual void modify(vertex&) = 0;
};

class render_conveyor
{
public:
    render_conveyor(canvas*, ivertex_shader*, ifragment_shader*);

    void clear(color);
    void switch_buffers();

    void draw_triangle(vertex v1, vertex v2, vertex v3);
    void draw_triangles(vertex_vector& vertices, index_vector& indexes);

    void set_pixel(pixel pixel);
    void draw_pixels(pixel_vector& pixels);

private:
    void rasterize_horizontal_line(pixel left, pixel right, pixel_vector& out);

    void rasterize_horizontal_triangle(pixel         left,
                                       pixel         right,
                                       pixel         v3,
                                       pixel_vector& out);

    void rasterize_triangle(pixel v1, pixel v2, pixel v3, pixel_vector& out);

    canvas*      main_renderer;
    color_vector secondary_buffer;
    int32_t      w;
    int32_t      h;

    ivertex_shader*   v_shader;
    ifragment_shader* f_shader;
};
