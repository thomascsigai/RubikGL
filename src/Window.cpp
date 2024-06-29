#include <window.hpp>

Window::Window(int width, int height, const std::string& name)
    : width(width), height(height), name(name), window(nullptr) {}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::init()
{
    if (!init_GLFW())
        return false;

    if (!init_GLEW())
        return false;

    return true;
}

bool Window::init_GLEW()
{
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }

    return true;
}

bool Window::init_GLFW()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    return true;
}

bool Window::should_close()
{
    return glfwWindowShouldClose(window);
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}