# ------------------------
# GENERAL PROJECT SETTINGS
# ------------------------

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) ???

cmake_minimum_required(VERSION 3.17.5)
project(Task6 C CXX)

# -----------------
# COMPILER SETTINGS
# -----------------

set(CMAKE_CXX_STANDARD 17)

# ------------
# DEPENDENCIES
# ------------

# OpenGL
find_package(OpenGL REQUIRED)

# GLM
add_subdirectory(libs/glm)

# GLFW
add_subdirectory(libs/glfw)

# STB_IMAGE
add_library(STB_IMAGE STATIC libs/stb_image/stb_image.cpp)
target_include_directories(STB_IMAGE PUBLIC libs/stb_image)

# GLAD
add_library(GLAD STATIC libs/glad/src/glad.c)
target_include_directories(GLAD PUBLIC libs/glad/include)

# IMGUI
set(IMGUI_DIR libs/imgui)
add_library(IMGUI STATIC)
target_sources( IMGUI
                PRIVATE
                    ${IMGUI_DIR}/imgui_demo.cpp
                    ${IMGUI_DIR}/imgui_draw.cpp
                    ${IMGUI_DIR}/imgui_tables.cpp
                    ${IMGUI_DIR}/imgui_widgets.cpp
                    ${IMGUI_DIR}/imgui.cpp
                    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
                    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
                )
target_include_directories( IMGUI
                            PUBLIC ${IMGUI_DIR}
                            PUBLIC ${IMGUI_DIR}/backends
                            PUBLIC libs/glfw/include
                            )
target_link_libraries(IMGUI PUBLIC GLAD glfw)

# ------------------
# PROJECT EXECUTABLE
# ------------------

file(GLOB_RECURSE MySources src/*.cpp libs/imgui)
add_executable(${PROJECT_NAME} ${MySources})
target_include_directories(${PROJECT_NAME} PUBLIC src libs)
target_link_libraries(${PROJECT_NAME} PUBLIC opengl32 glfw GLAD glm STB_IMAGE IMGUI)
