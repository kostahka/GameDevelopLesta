#include "01-render-object.hxx"

constexpr int32_t image_width  = 500;
constexpr int32_t image_heigth = 500;

constexpr color color_blue{ 0, 0, 255 };
constexpr color color_green{ 0, 255, 0 };
constexpr color color_red{ 255, 0, 0 };

int main()
{
    canvas image(image_width, image_heigth);

    image.clear(color_blue);

    render_object triangles;

    constexpr int32_t cell_size    = 50;
    constexpr size_t  cell_count_w = image_width / cell_size;
    constexpr size_t  cell_count_h = image_heigth / cell_size;

    position_vector vertices;

    constexpr size_t vertices_num = cell_count_w * cell_count_h * 2 * 3;
    vertices.reserve(vertices_num);

    for (size_t i = 0; i < cell_count_w; i++)
    {
        for (size_t j = 0; j < cell_count_h; j++)
        {
            position v1{ static_cast<int32_t>(i * cell_size),
                         static_cast<int32_t>(j * cell_size) };
            position v2{ v1.x + cell_size, v1.y };
            position v3{ v1.x + cell_size, v1.y + cell_size };
            position v4{ v1.x, v1.y + cell_size };

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            vertices.push_back(v3);
            vertices.push_back(v4);
            vertices.push_back(v1);
        }
    }

    triangles.draw_triangles_by_vertices(vertices, vertices_num, color_green);

    image.draw_render_object(triangles);

    std::string file_name = "02-triangle-render.ppm";
    image.save_image(file_name);

    return EXIT_SUCCESS;
}
