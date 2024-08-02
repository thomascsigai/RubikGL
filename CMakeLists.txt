cmake_minimum_required(VERSION 3.8)

project(RubikGL)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include/GLFW
    ${CMAKE_SOURCE_DIR}/include/GL
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/imgui/backends
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/shader
    ${CMAKE_SOURCE_DIR}/include/stb_image
)

# Copy shaders and textures to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources/shaders DESTINATION ${CMAKE_BINARY_DIR}/resources)
file(COPY ${CMAKE_SOURCE_DIR}/resources/textures DESTINATION ${CMAKE_BINARY_DIR}/resources)

# Add ImGui source files
set(IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
)

# Add GLFW library
add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/GLFW"
)

# Add GLEW static library
add_library(glew32s STATIC IMPORTED)
set_target_properties(glew32s PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/glew32s.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/GL"
)

# Check if GLFW library exists
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/glfw3.lib")
    message(FATAL_ERROR "GLFW library not found. Make sure glfw3.lib is in the 'lib' directory.")
endif()

# Check if GLEW static library exists
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/glew32s.lib")
    message(FATAL_ERROR "GLEW static library not found. Make sure glew32s.lib is in the 'lib' directory.")
endif()

find_package(OpenGL REQUIRED)

# Add source files to the executable
add_executable(RubikGL
    "src/Main.cpp"
    ${IMGUI_SRC}
    "src/Window.cpp"
    "src/Cube.cpp"
    "src/Piece.cpp"
    "src/stb_image.cpp"
    "src/GradientBackground.cpp"
)

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET RubikGL PROPERTY CXX_STANDARD 20)
endif()

target_compile_definitions(RubikGL PRIVATE GLEW_STATIC)
target_link_libraries(RubikGL PRIVATE glfw3 glew32s ${OPENGL_LIBRARIES})
