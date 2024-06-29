# CMakeList.txt : CMake project for RubikGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RubikGL")

# Ajouter le répertoire contenant les fichiers en-tête
include_directories(${CMAKE_SOURCE_DIR}/include/GLFW ${CMAKE_SOURCE_DIR}/include/GL ${CMAKE_SOURCE_DIR}/include/glm ${CMAKE_SOURCE_DIR}/include/imgui ${CMAKE_SOURCE_DIR}/include/imgui/backends
${CMAKE_SOURCE_DIR}/include/)

# Ajouter les fichiers source d'ImGui
set(IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
)

# Add GLFW Lib
add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/GLFW"
)

# Add GLEW Lib
add_library(glew32 STATIC IMPORTED)
set_target_properties(glew32 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/glew32.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/GL"
)

find_package(OpenGL REQUIRED)

# Add source to this project's executable.
add_executable (RubikGL "src/Main.cpp" ${IMGUI_SRC} "src/Window.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RubikGL PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(RubikGL PRIVATE glfw3 glew32 ${OPENGL_LIBRARIES})