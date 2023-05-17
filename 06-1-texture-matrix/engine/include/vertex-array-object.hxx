#pragma once

#include "vertex.hxx"

namespace Kengine
{

class static_vertex_array_object
{
public:
    virtual void draw_triangles(int count) = 0;
};

static_vertex_array_object* create_static_vao_color(const vertex_color_array&);

static_vertex_array_object* create_static_vao_text2d(
    const vertex_text2d_array&);

static_vertex_array_object* create_static_vao(const vertex_array&);

} // namespace Kengine
