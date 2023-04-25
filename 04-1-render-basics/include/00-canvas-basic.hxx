#pragma once

#include <cstdint>
#include <string>
#include <vector>

;
#pragma pack(push, 1)
struct color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    bool operator==(const color& other) const;
};
#pragma pack(pop)

struct position
{
    int32_t x;
    int32_t y;

    static position generate_random(int32_t width, int32_t height);

    bool operator==(const position& other) const;
};

struct pixel
{
    color    col;
    position pos;
};

using position_vector = std::vector<position>;
using color_vector    = std::vector<color>;
using pixel_vector    = std::vector<pixel>;

struct irender_object
{
    virtual const pixel_vector& get_pixels() const = 0;
};

class irenderer
{
public:
    virtual void clear(color)                              = 0;
    virtual void set_pixel(position, color)                = 0;
    virtual void draw_render_object(const irender_object&) = 0;
};

class canvas : public irenderer
{
public:
    canvas(int32_t width, int32_t height);

    void  clear(color) override;
    void  set_pixel(position, color) override;
    void  draw_render_object(const irender_object&) override;
    color get_pixel(position) const;

    bool save_image(const std::string& file_name) const;
    bool load_image(const std::string& file_name);

    int32_t get_width() const;
    int32_t get_height() const;

    bool operator==(const canvas& other) const;

    color_vector& get_buffer();

    color_vector::iterator begin();
    color_vector::iterator end();

private:
    color_vector pixels;
    int32_t      w;
    int32_t      h;
};
