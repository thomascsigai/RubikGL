#include <window.hpp>
#include <piece.hpp>

#include <glm.hpp>

int main(void)
{
    Window window;

    if (!window.init())
        return -1;

    Piece pieceTest = Piece(glm::vec3(1), glm::vec3(1));

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!window.should_close())
    {
        window.clear();

        pieceTest.draw();

        window.update();
    }

    return 0;
}