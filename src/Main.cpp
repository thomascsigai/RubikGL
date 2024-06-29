#include <window.hpp>

int main(void)
{
    Window window;

    if (!window.init())
        return -1;

    while (!window.should_close())
    {
        window.clear();


        window.update();
    }

    return 0;
}