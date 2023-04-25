#include "00-canvas-basic.hxx"
#include "04-render-conveyor.hxx"

constexpr int32_t image_width  = 500;
constexpr int32_t image_heigth = 500;

constexpr color color_blue{ 0, 0, 255 };
constexpr color color_green{ 0, 255, 0 };
constexpr color color_red{ 255, 0, 0 };
constexpr color color_black{ 0, 0, 0 };

class fragment_shader : public ifragment_shader
{
public:
    void modify(pixel& p) override { return; }
};

class vertex_shader : public ivertex_shader
{
public:
    void modify(vertex& v) override { return; };
};

int main()
{
    canvas image(image_width, image_heigth);

    image.clear(color_blue);

    fragment_shader f_shader;
    vertex_shader   v_shader;

    render_conveyor renderer(&image, &v_shader, &f_shader);

    vertex v1{ { 10, 10, 0 }, color_green };
    vertex v2{ { 10, 400, 0 }, color_blue };
    vertex v3{ { 250, 250, 0 }, color_red };

    renderer.clear(color_black);
    renderer.draw_triangle(v1, v2, v3);
    renderer.switch_buffers();

    std::string file_name = "04-interpolated-triangle.ppm";
    image.save_image(file_name);

    return EXIT_SUCCESS;
}
