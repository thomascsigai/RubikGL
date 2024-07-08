#include <window.hpp>

Window::Window(int width, int height, const std::string& name)
    : width(width), height(height), name(name), window(nullptr), io(nullptr) {}

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

    init_imGui();

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    return true;
}

bool Window::should_close()
{
    return glfwWindowShouldClose(window);
}

void Window::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::init_imGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::new_imGui_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::render_imGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::cleanup_imGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
  
void Window::draw_main_frame(Cube*& cube)
{
    new_imGui_frame();

    ImGui::Begin("RubikGL");

    ImGui::SliderInt("Cube Size", &settings.tempCubeSize, 2, 10);
    if (ImGui::Button("Generate"))
    {
        delete cube;
        cube = new Cube(settings.tempCubeSize);
        settings.cubeSize = settings.tempCubeSize;
    }

    ImGui::Checkbox("Show Polygons", &settings.showPolygons);
    if (settings.showPolygons)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ImGui::SliderFloat("Rotate Cube", &settings.rotationAngle, -180.0f, 180.0f);

    // Rotate Cube 45°
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { settings.rotationAngle -= 22.5f; }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { settings.rotationAngle += 22.5f; }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("Rotate Cube");

    ImGui::SliderFloat("Flip Cube", &settings.flipAngle, 0.0f, 180.0f);

    ImGui::SliderFloat("Camera Distance", &settings.zoom, 2.0f, 10.0f);

    init_rotate_ui(cube);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::End();

    render_imGui();
}

void Window::init_rotate_ui(Cube*& cube)
{
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;


    if (ImGui::Button("L"))
    {
        cube->rotate_face(0, false, col);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("L'"))
    {
        cube->rotate_face(0, true, col);
    }
    if (ImGui::Button("M"))
    {
        cube->rotate_face(1, false, col);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("M '"))
    {
        cube->rotate_face(1, true, col);
    }
    if (ImGui::Button("R"))
    {
        cube->rotate_face(2, false, col);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("R '"))
    {
        cube->rotate_face(2, true, col);
    }

    if (ImGui::Button("D"))
    {
        cube->rotate_face(0, false, line);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("D'"))
    {
        cube->rotate_face(0, true, line);
    }
    if (ImGui::Button("E"))
    {
        cube->rotate_face(1, false, line);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("E'"))
    {
        cube->rotate_face(1, true, line);
    }
    if (ImGui::Button("U"))
    {
        cube->rotate_face(2, false, line);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("U'"))
    {
        cube->rotate_face(2, true, line);
    }
    if (ImGui::Button("B"))
    {
        cube->rotate_face(0, false, face);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("B''"))
    {
        cube->rotate_face(0, true, face);
    }
    if (ImGui::Button("F"))
    {
        cube->rotate_face(2, false, face);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("F''"))
    {
        cube->rotate_face(2, true, face);
    }
}

SETTINGS Window::get_settings()
{
    return settings;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}