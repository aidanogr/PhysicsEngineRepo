//
// Created by Aidan O'Grady on 3/12/25.
//
#include "../include/Animate.h"

#include <stdio.h>

#include "GLFW/glfw3.h"




int frame_rate = 60;
int screen_width = 600;
int screen_height = 600;
/**
 * Changes default frame rate (60fps)
 * @param fps, fps must be a natural number (1, 2, 3, ...)
 * @return 0 if successful
 * @return -1 if fps not a natural number
 */
int setFrameRate(int fps) {
  if (fps <= 0) {
    return -1;
  }
  frame_rate = fps;
  return 0;
}

int setScreenSize(int width, int height) {
  screen_width = width;
  screen_height = height;
}


/**
 * *coordinates do not affect window size or position, only coordinate system for simulation*
 * @param x_min meters
 * @param y_min meters
 * @param x_max meters
 * @param y_max meters
 * @return 0 if successful
 * @return -1 if glfw failed to initialize
 * @return -2 if window failed to initialize (glfw gets terminated)
 */
int init_animation_2d(int x_min, int y_min, int x_max, int y_max) {
  if (!glfwInit())
  {
    printf("Failed to initialize GLFW\n");
    return -1;
  }
  window_handle = glfwCreateWindow(x_max, y_max, "Animate", NULL, NULL);
  if (window_handle == NULL) {
    glfwTerminate();
    printf("Failed to create GLFW window, glfwTerminated\n");
    return -2;
  }
  return 0;
}

int init_animation_3d(int x_min, int y_min, int x_max, int y_max) {
  //TODO
  return 0;
}

int draw(Universe* universe) {
  //TODO
  return 0;
}

int exportAnimation(char* filepath, char* filename) {
  //TODO
  return 0;
}

void terminate_animation() {
  //TODO
  glfwTerminate();
}

