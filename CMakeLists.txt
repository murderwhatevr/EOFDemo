cmake_minimum_required(VERSION 3.31)
project(EOF_1ST_DEMO)

set(CMAKE_CXX_STANDARD 26)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

# Include headers
include_directories(${Vulkan_INCLUDE_DIRS})

# Source files
add_executable(EOFDemo src/main.cpp
        src/VKRenderer.cpp
        src/VKRenderer.h
        src/Window.cpp
        src/Window.h
        src/Utilities.h
)

# Link libraries
target_link_libraries(EOFDemo
        imgui
        ${Vulkan_LIBRARIES}
        glfw
)

# Include directories
target_include_directories(EOFDemo PRIVATE
        src
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)