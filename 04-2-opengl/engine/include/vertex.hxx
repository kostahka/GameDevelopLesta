#pragma once

#include "color.hxx"
#include "transform3d.hxx"

#include <vector>

namespace Kengine
{

struct vertex
{
    transform3d pos;
    color       col;
};

using vertex_array = std::vector<vertex>;

} // namespace Kengine
