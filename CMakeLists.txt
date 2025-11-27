cmake_minimum_required(VERSION 3.10)
project(Transformation VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# Find GLFW using pkg-config (common on Linux)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(
    main 
    src/main.cpp 
    src/config.h
    src/glad.c
    src/triangle_mesh.h
    src/triangle_mesh.cpp
)

# Add include directories for GLAD
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dependencies)

# Link GLFW
target_link_libraries(main ${GLFW_LIBRARIES})
target_include_directories(main PRIVATE ${GLFW_INCLUDE_DIRS})
target_compile_options(main PRIVATE ${GLFW_CFLAGS_OTHER})
