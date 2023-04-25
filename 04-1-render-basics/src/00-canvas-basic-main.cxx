#include "00-canvas-basic.hxx"
#include <algorithm>
#include <iostream>

constexpr color   color_green{ 0, 255, 0 };
constexpr int32_t image_width  = 500;
constexpr int32_t image_height = 500;

int main()
{
    canvas image(image_width, image_height);

    uint8_t n = 0;
    std::generate(image.begin(),
                  image.end(),
                  [&n]()
                  {
                      n++;
                      return color{ static_cast<uint8_t>(n + 50),
                                    n,
                                    static_cast<uint8_t>(n + 100) };
                  });

    std::string file_name = "00-canvas-basic.ppm";
    image.save_image(file_name);

    canvas image_loaded(0, 0);
    image_loaded.load_image(file_name);

    if (image_loaded != image)
    {
        std::cerr << "Loaded image not equal to saved." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loaded image equal to saved." << std::endl;
    return EXIT_SUCCESS;
}
