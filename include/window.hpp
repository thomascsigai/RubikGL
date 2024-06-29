#ifndef H_WINDOW
#define H_WINDOW

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>

#define W_WIDTH 1600
#define W_HEIGHT 900
#define W_NAME "RubikGL"

class Window
{
public:

	Window(int width = W_WIDTH, int height = W_HEIGHT, const std::string& name = W_NAME);
	~Window();

	bool init();
	void clear();
	void update();
	bool should_close();

private:

	int width;
	int height;
	std::string name;
	GLFWwindow* window;

	bool init_GLFW();
	bool init_GLEW();
};

#endif