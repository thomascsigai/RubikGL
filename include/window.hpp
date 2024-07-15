#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glew.h>
#include <glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cube.hpp>
#include <settings.hpp>

#include <iostream>
#include <string>

#define W_WIDTH 1600
#define W_HEIGHT 900
#define W_NAME "RubikGL"

#define VIEW_ROTATION_SPEED 300.0f;

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

	void draw_ui_frames(Cube*& _cube);

	SETTINGS get_settings();
	GLfloat deltaTime;

private:

	int width;
	int height;
	std::string name;
	
	GLFWwindow* window;
	ImGuiIO* io;
	SETTINGS settings;
	GLfloat lastFrame;

	Cube* cube;

	bool init_GLFW();
	bool init_GLEW();

	bool viewRotating = false;
	float totalViewRotationAngle = 0;
	float currentViewRotationAngle = 0;
	float viewRotationSpeed = VIEW_ROTATION_SPEED;
	std::string viewRotDirection = "";
	bool keyDown = false;
	bool shiftDown = false;

	void start_view_rotation(std::string direction);
	void update_view_rotation();
	void processInput(int key, int scancode, int action, int mods);

	void draw_main_frame(Cube*& _cube);
	void draw_controls_frame();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif