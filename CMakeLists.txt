cmake_minimum_required(VERSION 3.31)
project(EOF_1ST_DEMO)

set(CMAKE_CXX_STANDARD 26)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include headers
include_directories(${Vulkan_INCLUDE_DIRS})

# Source files
add_executable(EOFDemo src/main.cpp
        src/VKRenderer.cpp
        src/VKRenderer.h
        src/Window.cpp
        src/Window.h
)

# Link libraries
target_link_libraries(EOFDemo
        ${Vulkan_LIBRARIES}
        glfw
)