#include <window.hpp>

int main(void)
{
    Window window;

    if (!window.init())
        return -1;

    Cube* cube = new Cube();

    while (!window.should_close())
    {
        window.clear();
        window.draw_ui_frames(cube);

        cube->draw(window.get_settings(), window.deltaTime);

        window.update();
    }
    window.cleanup_imGui();

    return 0;
}