#include <window.hpp>

int main(void)
{
    Window window;

    if (!window.init())
        return -1;

    Cube* cube = new Cube();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    while (!window.should_close())
    {
        window.clear();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.draw_main_frame(cube);
        cube->draw(window.get_settings(), deltaTime);

        window.update();
    }
    window.cleanup_imGui();

    return 0;
}