//
// Created by Aidan O'Grady on 3/17/25.
//

#ifndef ANIMATOR_H
#define ANIMATOR_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>


#define HEIGHT 1000
#define WIDTH 1000

extern int number_of_masses;
extern uint8_t image[HEIGHT][WIDTH][3];

extern double x_min;
extern double y_min;
extern double x_max;
extern double y_max;

extern double delta_width;
extern double delta_height;

extern int mapped_coordinate[2];

extern int x_axis_row;
extern int y_axis_column;

// Function declarations
int initialize_animation(double x_min_temp, double y_min_temp, double x_max_temp, double y_max_temp, int number_of_masses);
int change_background_color(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
int save_ppm(const char *filename, uint8_t image[HEIGHT][WIDTH][3]);
int map_coordinate_to_pixel(double coordinate_x, double coordinate_y);
int draw_tic_marks(int tic_width_percentage, int tic_height_percentage, double delta);
void draw_axis();
int draw_point_mass_by_pixel(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int position_x, int position_y);
int draw_point_mass_by_coordinate(uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y);
int draw_large_mass(uint8_t radius, uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y);

#endif //ANIMATOR_H
