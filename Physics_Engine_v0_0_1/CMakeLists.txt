cmake_minimum_required(VERSION 3.10)

project(Engine_v0_11)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)



add_executable(Engine ${SOURCES} SimulationsSource/EarthGravity.c)

