# ------------------------
# GENERAL PROJECT SETTINGS
# ------------------------

cmake_minimum_required(VERSION 3.17.5)
project(MarchingCompute C CXX)

set(PROJECT_SOURCE_DIR src)
set(PROJECT_INCLUDE_DIR src)
set(PROJECT_LIB_DIR libs)

# -----------------
# COMPILER SETTINGS
# -----------------

set(CMAKE_CXX_STANDARD 17)

# ------------
# DEPENDENCIES
# ------------

# GLAD
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)
list(APPEND PUBLIC_LIBRARIES glad)

# GLM
add_subdirectory(libs/glm)
list(APPEND PUBLIC_LIBRARIES glm)

# GLFW
add_subdirectory(libs/glfw)

# IMGUI
set(IMGUI_DIR libs/imgui)
add_library(imgui STATIC)
target_sources( imgui
        PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        )
target_include_directories(imgui
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC libs/glfw/include
        )
target_link_libraries(imgui PUBLIC glad glfw)
list(APPEND PRIVATE_LIBRARIES imgui)

# ------------------
# EXECUTABLE
# ------------------

file(GLOB_RECURSE MySources ${PROJECT_SOURCE_DIR}/**.cpp)
add_executable(${PROJECT_NAME} ${MySources})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_LIB_DIR}
        ${ADDITIONAL_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PUBLIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PRIVATE_LIBRARIES})
