//
// Created by Aidan O'Grady on 3/17/25.
//

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEIGHT 500
#define WIDTH 500

void save_ppm(const char *filename, unsigned char image[HEIGHT][WIDTH][3]);
int map_coordinate_to_pixel(double coordinate_x, double coordinate_y);
void draw_axis();

#endif //ANIMATOR_H
