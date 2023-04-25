#pragma once

#include "00-canvas-basic.hxx"

using index_vector = std::vector<uint16_t>;

struct render_object : irender_object
{
    const pixel_vector& get_pixels() const override;
    void                draw_line(position start, position end, color);
    void draw_triangle(position v1, position v2, position v3, color);
    void draw_triangles_by_vertices(position_vector& vertices,
                                    size_t           num_vertices,
                                    color);
    void draw_triangles_by_indexes(position_vector& vertices,
                                   index_vector&    indexes,
                                   color);

private:
    pixel_vector pixels;
};
