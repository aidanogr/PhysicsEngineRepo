//
// Created by Aidan O'Grady on 3/11/25.
//


#ifndef ANIMATE_H
#define ANIMATE_H

#include "UniverseHandler.h"
#include "GLFW/glfw3.h"

GLFWwindow* window_handle;

int init_animation_2d(int x_min, int y_min, int x_max, int y_max, char* window_name);
int init_animation_3d(int x_min, int y_min, int x_max, int y_max);
int draw(Universe* universe);
int exportAnimation(char* filePath, char* fileName);


#endif //ANIMATE_H
