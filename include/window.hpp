#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glew.h>
#include <glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cube.hpp>

#include <iostream>
#include <string>

#define W_WIDTH 1600
#define W_HEIGHT 900
#define W_NAME "RubikGL"

struct SETTINGS
{
	int cubeSize = 3;
	int tempCubeSize = 3;
	bool showPolygons = false;
};

class Window
{
public:

	Window(int width = W_WIDTH, int height = W_HEIGHT, const std::string& name = W_NAME);
	~Window();

	bool init();
	void clear();
	void update();
	bool should_close();

	void init_imGui();
	void new_imGui_frame();
	void render_imGui();
	void cleanup_imGui();

	void draw_main_frame(Cube*& cube);

private:

	int width;
	int height;
	std::string name;
	
	GLFWwindow* window;
	ImGuiIO* io;
	SETTINGS settings;

	bool init_GLFW();
	bool init_GLEW();
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif