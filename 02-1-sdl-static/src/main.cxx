#include <SDL3/SDL_version.h>
#include <cstdlib>
#include <iostream>
#include <ostream>

std::ostream& operator<<(std::ostream& output, const SDL_version& version)
{
    return output << static_cast<int>(version.major) << "."
                  << static_cast<int>(version.minor) << "."
                  << static_cast<int>(version.patch);
}

int main()
{
    SDL_version linked{ 0, 0, 0 };
    SDL_GetVersion(&linked);

    std::cout << linked << std::endl;

    return std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}
