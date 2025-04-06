cmake_minimum_required(VERSION 3.10)

project(UnitTests)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES ../src/*.c)
message(STATUS "Found source files:")
foreach(file ${SOURCES})
    message(STATUS "${file}")
endforeach()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(TESTS ${SOURCES} Simulation_Handler_Tester.c UniverseHandlerTester.c)

target_include_directories(TESTS PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_directories(TESTS PRIVATE ${GLFW_LIBRARY_DIRS})
target_link_libraries(TESTS ${GLFW_LIBRARIES} "-framework OpenGL")