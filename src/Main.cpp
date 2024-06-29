#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(1280, 720, "RubikGL", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Test GLM
    glm::vec3 v(1.0f, 0.0f, 0.0f);
    std::cout << "GLM vector: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;

    // render loop --------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
