#include <SDL3/SDL_version.h>
#include <cstdlib>
#include <iostream>
#include <ostream>

#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"

#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"

#define COLOR_RESET "\e[0m"

std::ostream& operator<<(std::ostream& output, const SDL_version& version)
{
    return output << GRN << static_cast<int>(version.major) << "." << YEL
                  << static_cast<int>(version.minor) << "." << RED
                  << static_cast<int>(version.patch) << COLOR_RESET;
}

int main()
{
    SDL_version compiled{ 0, 0, 0 };
    SDL_version linked{ 0, 0, 0 };

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << BHBLU << "Compiled version:" << compiled << std::endl;
    std::cout << BHMAG << "Linked version:" << linked << std::endl;

    return std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}
