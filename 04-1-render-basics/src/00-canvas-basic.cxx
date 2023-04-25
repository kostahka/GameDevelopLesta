#include "00-canvas-basic.hxx"
#include <fstream>

bool color::operator==(const color& other) const
{
    return r == other.r && g == other.g && b == other.b;
};

bool position::operator==(const position& other) const
{
    return x == other.x && y == other.y;
};

position position::generate_random(int32_t width, int32_t height)
{
    return { rand() % width, rand() % height };
}

canvas::canvas(int32_t width, int32_t height)
    : w(width)
    , h(height)
{
    pixels.resize(w * h);
};

void canvas::clear(color col)
{
    std::fill(pixels.begin(), pixels.end(), col);
}

color_vector& canvas::get_buffer()
{
    return pixels;
}

void canvas::set_pixel(position pos, color col)
{
    const int32_t index = w * pos.y + pos.x;
    pixels[index]       = col;
};

void canvas::draw_render_object(const irender_object& obj)
{
    for (const pixel& pix : obj.get_pixels())
    {
        if (pix.pos.x >= 0 && pix.pos.x < w && pix.pos.y >= 0 && pix.pos.y < h)
            set_pixel(pix.pos, pix.col);
    }
};

color canvas::get_pixel(position pos) const
{
    const int32_t index = w * pos.y + pos.x;
    return pixels[index];
};

static_assert(sizeof(color) == 3, "3 bytes of color");

bool canvas::save_image(const std::string& file_name) const
{
    std::ofstream file_out;
    file_out.open(file_name);

    file_out << "P6\n" << w << ' ' << h << '\n' << 255 << '\n';

    auto buf_size = static_cast<std::streamsize>(pixels.size() * sizeof(color));
    file_out.write(reinterpret_cast<const char*>(pixels.data()), buf_size);

    return file_out.good();
};

bool canvas::load_image(const std::string& file_name)
{
    std::ifstream file_in;
    file_in.open(file_name);

    std::string header;
    int         color_format;
    char        last_next_line = 0;

    // clang-format off
    file_in >> header
            >> w
            >> h
            >> color_format
            >> std::noskipws
            >> last_next_line;
    // clang-format on

    if (!iswspace(last_next_line))
        return false;

    if (header != "P6" || color_format > 255)
        return false;

    pixels.resize(w * h);

    auto buf_size = static_cast<std::streamsize>(pixels.size() * sizeof(color));
    file_in.read(reinterpret_cast<char*>(pixels.data()), buf_size);

    return file_in.good();
};

int32_t canvas::get_width() const
{
    return w;
};

int32_t canvas::get_height() const
{
    return h;
};

bool canvas::operator==(const canvas& other) const
{
    return pixels == other.pixels;
};

color_vector::iterator canvas::begin()
{
    return pixels.begin();
};

color_vector::iterator canvas::end()
{
    return pixels.end();
};
