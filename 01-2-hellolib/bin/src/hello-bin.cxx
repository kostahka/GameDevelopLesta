#include <cstdlib>
#include <hello-lib.hxx>
#include <iostream>

int main()
{
    bool good = hello_world();
    return good ? EXIT_SUCCESS : EXIT_FAILURE;
}
