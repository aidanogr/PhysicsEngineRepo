//
// Created by Aidan O'Grady on 3/12/25.
//
#include "../include/Animate.h"

#include <stdio.h>
#include <string.h>

#include "GLFW/glfw3.h"




int frame_rate = 60;
int screen_width = 600;
int screen_height = 600;


/**
 * Returns length of string by counting until null character is found.
 * If character is greater than >= 255, -1 is returned
 * @param str
 * @return length of string
 * @return -1 if null character isn't found
 */
int _strlen(char *str) {
  int count = 0;
  for (int i = 0; i < 255; i++) {
    if (str[i] != '\0') {
      count++;
    }
    else {
      return count;
    }
  }
  return -1;
}



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
 * -window name must be less than 255 characters
 * @param x_min meters
 * @param y_min meters
 * @param x_max meters
 * @param y_max meters
 * @param window_name (optional, for default pass NULL)
 * @return 0 if successful
 * @return -1 if glfw failed to initialize
 * @return -2 if window failed to initialize (glfw gets terminated)
 * @return -3 if window_name >= 255 characters
 */
int init_animation_2d(int x_min, int y_min, int x_max, int y_max, char* window_name) {
  if (!glfwInit())
  {
    printf("Failed to initialize GLFW\n");
    return -1;
  }
  if (window_name == NULL) {
    window_handle = glfwCreateWindow(x_max, y_max, "Simulation", NULL, NULL);
  }
  else {
      if (_strlen(window_name) < 255) {
        window_handle = glfwCreateWindow(x_max, y_max, window_name, NULL, NULL);
      }
      else {
        return -3;
      }
  }
  if (window_handle == NULL) {
    glfwTerminate();
    printf("Failed to create GLFW window, glfwTerminated\n");
    return -2;
  }
  glfwMakeContextCurrent(window_handle);
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window_handle))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window_handle);

    /* Poll for and process events */
    glfwPollEvents();
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
  glfwTerminate();
}

