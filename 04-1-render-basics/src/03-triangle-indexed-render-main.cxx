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

    constexpr int32_t cell_size    = 20;
    constexpr size_t  cell_count_w = image_width / cell_size;
    constexpr size_t  cell_count_h = image_heigth / cell_size;

    position_vector vertices;

    constexpr size_t vertices_num = (cell_count_w + 1) * (cell_count_h + 1);
    vertices.reserve(vertices_num);

    for (size_t i = 0; i <= cell_count_h; i++)
    {
        for (size_t j = 0; j <= cell_count_w; j++)
        {
            vertices.push_back({ static_cast<int32_t>(j * cell_size),
                                 static_cast<int32_t>(i * cell_size) });
        }
    }

    index_vector indexes;

    constexpr size_t indexes_num = cell_count_h * cell_count_w * 2 * 3;
    indexes.reserve(indexes_num);
    for (size_t i = 0; i < cell_count_h; i++)
    {
        for (size_t j = 0; j < cell_count_w; j++)
        {
            // clang-format off
            uint16_t index1 = static_cast<uint16_t>(i * (cell_count_w + 1) + j);
            uint16_t index2 = index1 + 1;
            uint16_t index3 = static_cast<uint16_t>((i + 1) * (cell_count_w + 1) + j + 1);
            uint16_t index4 = index3 - 1;
            // clang-format on

            indexes.push_back(index1);
            indexes.push_back(index2);
            indexes.push_back(index3);

            indexes.push_back(index3);
            indexes.push_back(index4);
            indexes.push_back(index1);
        }
    }

    triangles.draw_triangles_by_indexes(vertices, indexes, color_red);

    image.draw_render_object(triangles);

    std::string file_name = "03-triangle-indexed-render.ppm";
    image.save_image(file_name);

    return EXIT_SUCCESS;
}
