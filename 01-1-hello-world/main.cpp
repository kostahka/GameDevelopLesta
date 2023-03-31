#include <cstdlib>
#include <iostream>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    bool result = cout.good();
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
