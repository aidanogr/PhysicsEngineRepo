cmake_minimum_required(VERSION 3.10)

project(Engine_v0_11)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(Engine ${SOURCES} SimulationsSource/EarthGravity.c)

target_include_directories(Engine PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_directories(Engine PRIVATE ${GLFW_LIBRARY_DIRS})
#target_link_libraries(Engine ${GLFW_LIBRARIES} "-framework OpenGL")