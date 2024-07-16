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
    glfwSetWindowPos(window, 100, 100);
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
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mygl_GradientBackground(0.59, 0.74, 1.0, 1.0,
        0.87, 0.89, 1.0, 1.0);

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

void Window::draw_ui_frames(Cube*& _cube)
{
    cube = _cube;

    new_imGui_frame();

    draw_main_frame(_cube);
    draw_controls_frame();
    draw_cube_infos_frame(_cube);

    render_imGui();
}
  
void Window::draw_main_frame(Cube*& _cube)
{
    ImGui::Begin("RubikGL");

    ImGui::SliderInt("Cube Size", &settings.tempCubeSize, 2, 5);
    if (ImGui::Button("Generate"))
    {
        delete _cube;
        _cube = new Cube(settings.tempCubeSize);
        settings.cubeSize = settings.tempCubeSize;

        cube = _cube;
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

    if (ImGui::Button("Scramble"))
    {
        cube->scramble();
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);

    ImGui::End();
}

void Window::draw_controls_frame()
{
    ImGui::Begin("Controls");

    ImGui::SeparatorText("LINES:");
    ImGui::Text("U -> Up");
    ImGui::Text("LSHIFT + U -> Inverse Up");
    ImGui::Text("D -> Down");
    ImGui::Text("LSHIFT + D -> Inverse Down");
    ImGui::Text("E -> Equator");
    ImGui::Text("LSHIFT + E -> Inverse Equator");

    ImGui::SeparatorText("COLUMNS:");
    ImGui::Text("L -> Left");
    ImGui::Text("LSHIFT + L -> Inverse Left");
    ImGui::Text("M -> Middle");
    ImGui::Text("LSHIFT + M -> Inverse Middle");
    ImGui::Text("R -> Right");
    ImGui::Text("LSHIFT + R -> Inverse Right");

    ImGui::SeparatorText("FACES:");
    ImGui::Text("F -> Front");
    ImGui::Text("LSHIFT + F -> Inverse Front");
    ImGui::Text("B -> Back");
    ImGui::Text("LSHIFT + B -> Inverse Back");

    ImGui::SeparatorText("LINES:");
    ImGui::Text("RIGHT ARROW -> Rotate Right");
    ImGui::Text("LEFT ARROW -> Rotate Left");
    ImGui::Text("UP ARROW -> Flip Cube");
    ImGui::Text("DOWN ARROW -> Flip Cube");

    ImGui::End();
}

void Window::draw_cube_infos_frame(Cube*& _cube)
{
    ImGui::Begin("Cube Infos");

    ImGui::Text("Last move : %s", lastMove.c_str());
    ImGui::Text("Number of Moves : %u", cube->numberOfMoves);
    
    ImGui::End();
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

            cube->rotate_face(faceIndex, shiftDown, dir);
            
            lastMove = "Left";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;

            cube->numberOfMoves++;
            
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

            cube->rotate_face(faceIndex, shiftDown, dir);
            lastMove = "Middle";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
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
            lastMove = "Right";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
        }

        if (key == GLFW_KEY_U)
        {
            dir = line;

            if ((int)round(settings.flipAngle) / 180 % 2 == 0)
                faceIndex = 2;
            else
                shiftDown = !shiftDown;

            cube->rotate_face(faceIndex, shiftDown, dir);
            lastMove = "Up";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
        }

        if (key == GLFW_KEY_E)
        {
            dir = line;

            faceIndex = 1;

            if ((int)round(settings.flipAngle) / 180 % 2 != 0)
                shiftDown = !shiftDown;

            cube->rotate_face(faceIndex, shiftDown, dir);
            lastMove = "Equator";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
        }

        if (key == GLFW_KEY_D)
        {
            dir = line;

            if ((int)round(settings.flipAngle) / 180 % 2 != 0)
            {
                faceIndex = 2;
                shiftDown = !shiftDown;
            }

            cube->rotate_face(faceIndex, shiftDown, dir);
            lastMove = "Down";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
        }

        if ((int)round(settings.rotationAngle) % 180 == 40 || (int)round(settings.rotationAngle) % 180 == -140)
            dir = face;
        else
            dir = col;

        if (key == GLFW_KEY_B)
        {
            if ((int)round(settings.flipAngle) / 180 % 2 == 0)
            {
                if (rotIndex == 2 || rotIndex == 3)
                    faceIndex = 2;
                if (rotIndex == 2 || rotIndex == 1)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    faceIndex == 0 ? faceIndex = 2 : faceIndex = 0;
            }
            else
            {
                if (rotIndex == 0 || rotIndex == 3)
                    faceIndex = 2;
                if (rotIndex == 0 || rotIndex == 1)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    shiftDown = !shiftDown;
            }

            cube->rotate_face(faceIndex, shiftDown, dir);
            lastMove = "Back";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
        }

        if (key == GLFW_KEY_F)
        {
            if ((int)round(settings.flipAngle) / 180 % 2 == 0)
            {
                if (rotIndex == 1 || rotIndex == 0)
                    faceIndex = 2;
                if (rotIndex == 2 || rotIndex == 1)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    faceIndex == 0 ? faceIndex = 2 : faceIndex = 0;
            }
            else
            {
                if (rotIndex == 1 || rotIndex == 2)
                    faceIndex = 2;
                if (rotIndex == 0 || rotIndex == 1)
                    shiftDown = !shiftDown;
                if (settings.rotationAngle < 0)
                    shiftDown = !shiftDown;
            }

            cube->rotate_face(faceIndex, shiftDown, dir);
            lastMove = "Front";
            if (shiftDown)
                lastMove = "Inverse " + lastMove;
            cube->numberOfMoves++;
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