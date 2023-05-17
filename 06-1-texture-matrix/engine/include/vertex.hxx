#pragma once

#include "color.hxx"
#include "transform3d.hxx"

#include <vector>

namespace Kengine
{

struct vertex
{
    transform3d pos;
};

struct vertex_color
{
    transform3d pos;
    color       col;
};

struct vertex_text2d
{
    transform3d pos;
    transform2d text;
};

using vertex_array        = std::vector<vertex>;
using vertex_color_array  = std::vector<vertex_color>;
using vertex_text2d_array = std::vector<vertex_text2d>;

} // namespace Kengine
