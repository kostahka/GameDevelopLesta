#pragma once

#include "00-canvas-basic.hxx"

struct position_3d
{
    double x;
    double y;
    double z;
};

position_3d operator+(const position_3d& p1, const position_3d& p2);

struct vertex
{
    position_3d pos;
    color       col{ 0, 0, 0 };
};

using vertex_vector = std::vector<vertex>;
using index_vector  = std::vector<uint16_t>;

struct uniforms
{
    double f1;
    double f2;
    double f3;
    double f4;
};

struct irender_program
{
    virtual void   set_uniforms(const uniforms&) = 0;
    virtual vertex vertex_shader(const vertex&)  = 0;
    virtual color  fragment_shader(const pixel&) = 0;
};

class render_conveyor
{
public:
    render_conveyor(canvas*);

    void set_program(irender_program*);

    void clear(color);
    void switch_buffers();

    void draw_triangle(vertex& v1, vertex& v2, vertex& v3);
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

    irender_program* program = nullptr;
};

position    interpolate(const position& p1, const position& p2, const double t);
position_3d interpolate(const position_3d& p1,
                        const position_3d& p2,
                        const double       t);
color       interpolate(const color& c1, const color& c2, const double t);
vertex      interpolate(const vertex& v1, const vertex& v2, const double t);
pixel       interpolate(const pixel& p1, const pixel& p2, const double t);
