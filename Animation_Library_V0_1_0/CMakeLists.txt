cmake_minimum_required(VERSION 3.30)

project(AnimationLibrary C)
set(CMAKE_C_STANDARD 11)

# Step 1: Build the library from your source files (excluding Java_CLI_Handler and any test main)
add_library(animation STATIC
    frame_drawer.c
    animator.c
)

# Optionally include headers (for IDEs and proper installation)
target_include_directories(animation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Step 2: Build a test executable for development (this can be removed in production)
add_executable(AnimationLibraryTester main_tester.c)
target_link_libraries(AnimationLibraryTester animation)

# Step 3: Build the Java CLI handler version as a separate executable or shared library (depending on your needs)
add_library(Java_CLI SHARED Java_CLI_Handler.c)
target_link_libraries(Java_CLI animation)

# You can install or output the library with proper settings:
set_target_properties(Java_CLI PROPERTIES OUTPUT_NAME "java_cli_handler")

