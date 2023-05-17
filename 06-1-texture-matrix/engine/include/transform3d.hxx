#pragma once

namespace Kengine
{
struct transform2d
{
    float x;
    float y;
};
struct transform3d
{
    float x;
    float y;
    float z;

    static transform3d blend(const transform3d& t1,
                             const transform3d& t2,
                             const float        a)
    {
        float x = (1.0f - a) * t1.x + a * t2.x;
        float y = (1.0f - a) * t1.y + a * t2.y;
        float z = (1.0f - a) * t1.z + a * t2.z;
        return { x, y, z };
    };
};
} // namespace Kengine
