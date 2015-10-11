#include "inverse.h"

int main()
{
    inverse bla;
    bla.input("area.txt", "receivers.txt", "config.txt");
    bla.calc();

    cout << "Hello World!" << endl;
#if defined _WIN32
    system("pause");
#endif
    return 0;
}