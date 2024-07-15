#include <window.hpp>

Window::Window(int width, int height, const std::string& name)
    : width(width), height(height), name(name), window(nullptr), io(nullptr), deltaTime(0.0f), lastFrame(0.0f), cube(nullptr) {}

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
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);

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

    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    update_view_rotation();
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
  
void Window::draw_main_frame(Cube*& _cube)
{
    cube = _cube;

    new_imGui_frame();

    ImGui::Begin("RubikGL");

    ImGui::SliderInt("Cube Size", &settings.tempCubeSize, 2, 5);
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

    init_rotate_ui();

    if (ImGui::Button("Scramble"))
    {
        cube->scramble();
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::End();

    render_imGui();
}

void Window::init_rotate_ui()
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
    if (ImGui::Button("M'"))
    {
        cube->rotate_face(1, true, col);
    }
    if (ImGui::Button("R"))
    {
        cube->rotate_face(2, false, col);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("R'"))
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
    if (ImGui::Button("B'"))
    {
        cube->rotate_face(0, true, face);
    }
    if (ImGui::Button("F"))
    {
        cube->rotate_face(2, false, face);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("F'"))
    {
        cube->rotate_face(2, true, face);
    }
}

SETTINGS Window::get_settings()
{
    return settings;
}

void Window::processInput(int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        shiftDown = true;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        shiftDown = false;

    

    if (!viewRotating && action == GLFW_PRESS)
    {
        RotateDirection dir;
        int faceIndex = 0;
        // rotIndex value : 0 = orange face front, 1 = blue face front, ... 
        int rotIndex = abs((int)(round(settings.rotationAngle) / 90) % 4);

        if ((int)round(settings.rotationAngle) % 180 == 40 || (int)round(settings.rotationAngle) % 180 == -140)
            dir = col;
        else
            dir = face;

        if (key == GLFW_KEY_L)
        {
            if ((int)round(settings.flipAngle) / 180 % 2 == 0)
            {
                if (rotIndex == 1 || rotIndex == 2)
                    faceIndex = 2;
                if (rotIndex == 2 || rotIndex == 3)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    shiftDown = !shiftDown;
            }
            else
            {
                if (rotIndex == 3 || rotIndex == 2)
                    faceIndex = 2;
                if (settings.rotationAngle < 0)
                    faceIndex == 0 ? faceIndex = 2 : faceIndex = 0;
                if (rotIndex == 1 || rotIndex == 2)
                    shiftDown = !shiftDown;
            }

            if ((int)round(settings.rotationAngle) % 180 == 40 || (int)round(settings.rotationAngle) % 180 == -140)
                dir = col;
            else
                dir = face;

            cube->rotate_face(faceIndex, shiftDown, dir);
        }
        if (key == GLFW_KEY_M)
        {
            faceIndex = 1;

            if ((int)round(settings.flipAngle) / 180 % 2 == 0)
            {
                if (rotIndex == 2 || rotIndex == 3)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    shiftDown = !shiftDown;
            }
            else
            {
                if (rotIndex == 1 || rotIndex == 2)
                    shiftDown = !shiftDown;
            }

            if ((int)round(settings.rotationAngle) % 180 == 40 || (int)round(settings.rotationAngle) % 180 == -140)
                dir = col;
            else
                dir = face;

            cube->rotate_face(faceIndex, shiftDown, dir);
        }
        if (key == GLFW_KEY_R)
        {
            if ((int)round(settings.flipAngle) / 180 % 2 == 0)
            {
                if (rotIndex == 0 || rotIndex == 3)
                    faceIndex = 2;
                if (rotIndex == 2 || rotIndex == 3)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    shiftDown = !shiftDown;
            }
            else
            {
                if (rotIndex == 3 || rotIndex == 2)
                    faceIndex = 2;
                if (settings.rotationAngle > 0)
                    faceIndex == 0 ? faceIndex = 2 : faceIndex = 0;
                if (rotIndex == 1 || rotIndex == 2)
                    shiftDown = !shiftDown;
            }

            cube->rotate_face(faceIndex, shiftDown, dir);
        }
    }

    if (!keyDown && !viewRotating)
    {
        if (key == GLFW_KEY_RIGHT)
            start_view_rotation("Right");
        if (key == GLFW_KEY_LEFT)
            start_view_rotation("Left");
        if (key == GLFW_KEY_DOWN)
            start_view_rotation("Down");
        if (key == GLFW_KEY_UP)
            start_view_rotation("Up");
    }

    if (action == GLFW_PRESS)
        keyDown = true;

    if (action == GLFW_RELEASE)
        keyDown = false;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->processInput(key, scancode, action, mods);
    }
}

void Window::start_view_rotation(std::string direction)
{
    if (viewRotating) false;

    viewRotating = true;
    currentViewRotationAngle = 0.0f;
    totalViewRotationAngle = 90.0f;
    viewRotDirection = direction;
        
    if (direction == "Down" || direction == "Left")
    {
        totalViewRotationAngle *= -1;
        viewRotationSpeed *= -1;
    }

    if (direction == "Down" || direction == "Up")
    {
        totalViewRotationAngle *= 2;
    }
}

void Window::update_view_rotation()
{
    if (!viewRotating) return;

    float angleStep = viewRotationSpeed * deltaTime;
    currentViewRotationAngle += angleStep;

    // stop condition
    if ((angleStep >= 0 && currentViewRotationAngle >= totalViewRotationAngle) ||
        (angleStep <= 0 && currentViewRotationAngle <= totalViewRotationAngle))
    {
        angleStep -= (currentViewRotationAngle - totalViewRotationAngle);
        viewRotating = false;
        viewRotationSpeed = abs(viewRotationSpeed);
    }

    if (viewRotDirection == "Up")
        settings.flipAngle += angleStep;
    if (viewRotDirection == "Down")
        settings.flipAngle += angleStep;
    if (viewRotDirection == "Left")
        settings.rotationAngle += angleStep;
    if (viewRotDirection == "Right")
        settings.rotationAngle += angleStep;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}