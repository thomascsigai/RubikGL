![RubikGL Banner](https://drive.google.com/uc?export=view&id=1MANbBuLu-vMfRPue6ifR_T8wZKqHq-yQ)

# RubikGL

RubikGL is an interactive 3D Rubik's Cube simulator built with OpenGL and CMake. It features customizable cube sizes, face rotations, scrambling, and camera controls.
Made to learn more about computer graphics programming using C++ and OpenGL.

## Features

- **Customizable Cube Size**: Generate a Rubik's Cube of any size.
- **Face Rotation**: Interactively rotate the cube's faces.
- **Scrambling**: Randomly scramble the cube.
- **Camera Controls**: Rotate and zoom the camera to view the cube from different perspectives.
- **User Interface**: Integrated with ImGui for an intuitive user interface.

## Demo

A demo of RubikGL can be found on YouTube. [Demo Video]([#](https://www.youtube.com/watch?v=0ZmnrCsjMKA&ab_channel=ThomasCsigai))

## Prerequisites

- CMake (version 3.10 or higher)

## Installation (Windows with Visual Studio)

1. **Clone the repository:**
    ```bash
    git clone https://github.com/thomascsigai/RubikGL.git
    cd RubikGL
    ```

2. **Generate Visual Studio project files:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3. **Open the generated solution file:**
    - Navigate to the `build` directory.
    - Open `RubikGL.sln` with Visual Studio.

4. **Build the project:**
    - Set the build configuration to `Release` or `Debug`.
    - Build project `ALL_BUILD` (In Solution explorer Right-click on `ALL_BUILD` / Build).

## Usage (In Visual Studio)

1. **Set RubikGL as startup project:**
   - In Solution explorer Right-click on `RubikGL` / Set as startup project
  
2. **Run the app:**
   - Run the `RubikGL` project (`F5`)
   - Use the interface to customize the cube, rotate faces, scramble the cube, and adjust the camera.

Controls are shown in the controls window, and the cube can be changed in the cube settings window. You may have to move the windows a bit to see all options clearly.

## Documentation

| File                   | Summary                                                   |
|------------------------|-----------------------------------------------------------|
| `Cube.cpp`             | Handles the creation and manipulation of the Rubik's Cube.|
| `GradientBackground.cpp` | Manages the gradient background rendering.              |
| `Main.cpp`             | Entry point of the application, initializes and runs the main loop. |
| `Piece.cpp`            | Defines individual pieces of the Rubik's Cube.            |
| `stb_image.cpp`        | Third-party library for loading images.                   |
| `Window.cpp`           | Manages the window creation and input handling.           |

## Acknowledgements

- **OpenGL**: The core graphics library used for rendering.
- **GLFW**: For creating windows and handling input.
- **GLEW**: For managing OpenGL extensions.
- **GLM**: For mathematical operations.
- **ImGui**: For the user interface.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions or suggestions, don't hesitate to contact me.

### Social Media

- [LinkedIn](https://www.linkedin.com/in/thomas-csigai/)
- [Itch.io](https://thomas-csigai.itch.io/)
- Email: thomascsigai1@gmail.com
