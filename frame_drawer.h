//
// Created by Aidan O'Grady on 3/17/25.
//

#ifndef FRAME_DRAWER_H
#define FRAME_DRAWER_H


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

extern uint16_t mapped_coordinate[2];

extern int x_axis_row;
extern int y_axis_column;

// Function declarations
uint8_t initialize_frame(double x_min_temp, double y_min_temp, double x_max_temp, double y_max_temp, int number_of_masses);
uint8_t change_background_color(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
void change_coordinate_system_color(uint8_t RED, uint8_t GREEN, uint8_t BLUE); 
uint8_t save_ppm(const char *filename, uint8_t image[HEIGHT][WIDTH][3]);
uint8_t map_coordinate_to_pixel(double coordinate_x, double coordinate_y);
uint8_t draw_tic_marks(int tic_width_percentage, int tic_height_percentage, double delta);
void draw_axis();
uint8_t draw_point_mass_by_pixel(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int position_x, int position_y);
uint8_t draw_point_mass_by_coordinate(uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y);
uint8_t draw_large_mass(uint8_t radius, uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y);

#endif //ANIMATOR_H
