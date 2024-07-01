#include <window.hpp>
#include <cube.hpp>

int main(void)
{
    Window window;

    if (!window.init())
        return -1;

    Cube* cube = new Cube();

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!window.should_close())
    {
        window.clear();

        window.draw_main_frame(cube);
        cube->draw();

        window.update();
    }
    window.cleanup_imGui();

    return 0;
}