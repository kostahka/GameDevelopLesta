#include "01-render-object.hxx"

const pixel_vector& render_object::get_pixels() const
{
    return pixels;
};

enum class line_direction
{
    right,
    up,
    left,
    down
};

void render_object::draw_line(position start, position end, color col)
{
    line_direction direction;

    int delta_x = std::abs(start.x - end.x);
    int delta_y = std::abs(start.y - end.y);

    if (delta_x > delta_y)
    {
        if (start.x < end.x)
            direction = line_direction::right;
        else
            direction = line_direction::left;
    }
    else
    {
        if (start.y < end.y)
            direction = line_direction::up;
        else
            direction = line_direction::down;
    }

    int32_t x_start;
    int32_t x_end;
    int32_t y_start;
    int32_t y_end;

    switch (direction)
    {
        case line_direction::right:
            x_start = start.x;
            x_end   = end.x;
            y_start = start.y;
            y_end   = end.y;
            break;
        case line_direction::left:
            x_start = end.x;
            x_end   = start.x;
            y_start = end.y;
            y_end   = start.y;
            break;
        case line_direction::up:
            x_start = start.y;
            x_end   = end.y;
            y_start = start.x;
            y_end   = end.x;
            break;
        case line_direction::down:
            x_start = end.y;
            x_end   = start.y;
            y_start = end.x;
            y_end   = start.x;
            break;
    };

    int32_t x_delta = std::abs(x_start - x_end);
    int32_t y_delta = std::abs(y_start - y_end);

    int error       = 0;
    int delta_error = y_delta + 1;

    int32_t y     = y_start;
    int32_t dir_y = y_end > y_start ? 1 : -1;

    for (int32_t x = x_start; x <= x_end; x++)
    {
        if (direction == line_direction::right ||
            direction == line_direction::left)
            pixels.push_back(pixel{ col, { x, y } });
        else
            pixels.push_back(pixel{ col, { y, x } });

        error += delta_error;
        if (error >= x_delta + 1)
        {
            y += dir_y;
            error -= x_delta + 1;
        }
    }
};

void render_object::draw_triangle(position v1,
                                  position v2,
                                  position v3,
                                  color    col)
{
    draw_line(v1, v2, col);
    draw_line(v2, v3, col);
    draw_line(v3, v1, col);
};

void render_object::draw_triangles_by_vertices(position_vector& vertices,
                                               size_t           num_vertices,
                                               color            col)
{
    for (size_t i = 0; i < num_vertices / 3; i++)
    {
        position v1 = vertices.at(i * 3 + 0);
        position v2 = vertices.at(i * 3 + 1);
        position v3 = vertices.at(i * 3 + 2);

        draw_triangle(v1, v2, v3, col);
    }
};

void render_object::draw_triangles_by_indexes(position_vector& vertices,
                                              index_vector&    indexes,
                                              color            col)
{
    for (size_t i = 0; i < indexes.size() / 3; i++)
    {
        size_t index1 = indexes.at(i * 3 + 0);
        size_t index2 = indexes.at(i * 3 + 1);
        size_t index3 = indexes.at(i * 3 + 2);

        position& v1 = vertices.at(index1);
        position& v2 = vertices.at(index2);
        position& v3 = vertices.at(index3);

        draw_triangle(v1, v2, v3, col);
    }
};
