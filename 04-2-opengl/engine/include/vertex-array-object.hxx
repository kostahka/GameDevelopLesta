#pragma once

#include "vertex.hxx"

namespace Kengine
{

class vertex_array_object
{
public:
    virtual void draw_triangles(int count) = 0;
};

extern vertex_array_object* create_vertex_array_object(const vertex_array&);
} // namespace Kengine
