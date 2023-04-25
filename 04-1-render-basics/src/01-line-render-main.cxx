#include "01-render-object.hxx"

constexpr uint32_t image_width  = 500;
constexpr uint32_t image_heigth = 500;

constexpr color color_blue{ 0, 0, 255 };
constexpr color color_green{ 0, 255, 0 };
constexpr color color_red{ 255, 0, 0 };

int main()
{
    canvas image(image_width, image_heigth);

    render_object lines;

    image.clear(color_blue);

    lines.draw_line({ 0, 0 }, { 499, 499 }, color_green);
    lines.draw_line({ 400, 100 }, { 100, 400 }, color_green);
    lines.draw_line({ 250, 100 }, { 250, 400 }, color_green);

    for (int i = 0; i < 100; i++)
    {
        position start{ position::generate_random(image_width, image_heigth) };
        position end{ position::generate_random(image_width, image_heigth) };

        lines.draw_line(start, end, color_red);
    }

    image.draw_render_object(lines);

    std::string file_name = "01-line-render.ppm";
    image.save_image(file_name);

    return EXIT_SUCCESS;
}
