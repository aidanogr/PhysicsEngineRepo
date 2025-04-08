#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include "frame_drawer.h"





int number_of_masses = 0;

double x_min;
double y_min;
double x_max;
double y_max;

uint8_t image[HEIGHT][WIDTH][3] = {0};
double delta_width;
double delta_height;

uint16_t mapped_coordinate[] = {0, 0};

uint8_t COORDINATE_SYSTEM_RED;
uint8_t COORDINATE_SYSTEM_BLUE;
uint8_t COORDINATE_SYSTEM_GREEN;

//-1 meaning "unset" (see draw_axis())
int x_axis_row = -1;
int y_axis_column = -1;

//each bit represents a state. Set in draw_axis(), used in draw_tic_marks() to avoid weird behavior when
//picture does not show the origin.
//[reserved..(always zero)][y_min>=0][y_max<=0][x_min>=0][x_max<=0]
int8_t boundary_flags = 0b0;




/**
 * pretty self explanatory
 * @param y y pixel in image
 * @param x x pixel in image
 * @param RED rgb value for red
 * @param BLUE rgb value for blue
 * @param GREEN rgb value for green
 * @return -1 if x,y is outside range of image 
 * */
int set_pixel_RGB(int y, int x, uint8_t RED, uint8_t BLUE, uint8_t GREEN) {
    if(y < 0 || y >= HEIGHT || x < 0 || x > WIDTH) {
	return -1;
    }
    image[y][x][0] = RED;
    image[y][x][1] = BLUE;
    image[y][x][2] = GREEN;

    return 0;
}


/**
 * background color is black by default (0,0,0).
 * Note: must call this function immediately after initialize_frame()
 */
int8_t change_background_color(uint8_t RED, uint8_t BLUE, uint8_t GREEN) {

    for(int y = 0; y < HEIGHT; y++) {
	for(int x = 0; x < WIDTH; x++) {
	    set_pixel_RGB(y, x, RED, BLUE, GREEN);
	}
    }
    return 0;
}


/**
 * Bounds must have 2 dimensional shape. (min != max, min < max, HEIGHT and WIDTH > 0).
 * Do not attempt to force lower dimensional shape (squeezing everything onto a single axis).
 * TODO create 1d and 3d animation initializers / functionality
 * MUST CALL THIS FUNCTION BEFORE USING ANY OTHER   
 * @return 0 if successful
 * @return -1 if bounds have bad shape (x_min >= x_max or y_min >= y_max)
 * @return -2 if WIDTH <= 0 or HEIGHT <= 0
 */
int8_t initialize_frame(double x_min_temp, double y_min_temp, double x_max_temp, double y_max_temp, int number_of_masses_temp) {
    if(x_min_temp >= x_max_temp || y_min_temp >= y_max_temp) {
	return -1;
    }
    if(WIDTH <= 0 || HEIGHT <= 0) {
	return -2;
    }
    number_of_masses = number_of_masses_temp;
    x_min = x_min_temp;
    y_min = y_min_temp;
    x_max = x_max_temp;
    y_max = y_max_temp;
    
    delta_width = (x_max-x_min)/WIDTH;
    delta_height = (y_max-y_min)/HEIGHT;

    COORDINATE_SYSTEM_RED = 250;
    COORDINATE_SYSTEM_BLUE = 0;
    COORDINATE_SYSTEM_GREEN = 0;

//    mapped_coordinate[0] = 0; mapped_coordinate[1] = 0;
    x_axis_row = -1;
    y_axis_column = -1;
    change_background_color(0, 0, 0);
    boundary_flags &= 0b0;
    
    
    return 0;
}

/**
 * Used to update axis and tic mark colors.
 * By default, the color will always be 250, 0, 0
 */
void change_coordinate_system_color(uint8_t RED, uint8_t GREEN, uint8_t BLUE) {
    COORDINATE_SYSTEM_RED = RED;
    COORDINATE_SYSTEM_GREEN = GREEN;
    COORDINATE_SYSTEM_BLUE = BLUE;
}
    


/**
 * //TODO this is written by deepseek, make sure it isn't a backdoor from riot games
 * @return 0 if file saved successfully
 * @return -1 if unsuccessful
 */
int8_t save_ppm(const char *filename, uint8_t image[HEIGHT][WIDTH][3]) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Failed to open file for writing: %s\n", filename);
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, 1, WIDTH * HEIGHT * 3, fp);
    fclose(fp);
    return 0;
}


//TODO unit test this function
/**
 * updates mapped_coordinate[x,y]
 * @param coordinate_x
 * @param coordinate_y
 *
 * @return 0 if successful
 * @return -1 if unsuccessful, mapped_coordinate
 */
int8_t map_coordinate_to_pixel(double coordinate_x, double coordinate_y) {
    if(coordinate_y < y_min || coordinate_y > y_max || coordinate_x < x_min || coordinate_x > x_max) {
	//printf("Tried to map coordinate outside of range: y:%.2f, x:%.2f\n", coordinate_y, coordinate_x);
  	return -1;
    }
    mapped_coordinate[0] = (int) ((coordinate_x - x_min) / delta_width);
    mapped_coordinate[1] = HEIGHT - (int) ((coordinate_y - y_min) / delta_height);
    return 0;
}


//make color changeable
/**
 * Always call draw_axis() before draw_tic***
 * note: tic width represents lengths of tic either horizontal 
 *	 or vertical, these are not the dimesions of a single tic. By default, thickness of 
 *	 tic is 1 pixel. //TODO add variable tic thickness
 * @param tic_width_percentage : width of tic expressed as percentage of width
 * @param tic_height_percentage : height of tic expressed as percentage of height
 * @param delta : coordinate difference between tic marks (not pixel difference) 
 * @return //TODO does this need a return?
*/
int8_t draw_tic_marks(int tic_width_horizontal_percentage, int tic_width_vertical_percentage, double delta) {

    int tic_width_horizontal = (int) (0.01 * tic_width_horizontal_percentage * WIDTH);
    int tic_width_vertical = (int) (0.01 * tic_width_vertical_percentage * HEIGHT);
    int number_of_tics_y_axis = (int) ((y_max-y_min)/delta);
    int number_of_tics_x_axis = (int) ((x_max-x_min)/delta);

    //y-axis tics
    for(int y = 0; y < number_of_tics_y_axis; y++) {
	if((boundary_flags & 0b0010) > 0) {
	    map_coordinate_to_pixel(x_min, y_min + y * delta);
	    for(int x = mapped_coordinate[0]; x < mapped_coordinate[0] + tic_width_horizontal/2; x++) {
		set_pixel_RGB(mapped_coordinate[1]-1, x, COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_BLUE, COORDINATE_SYSTEM_GREEN);
	    }
	}
	else if((boundary_flags & 0b0001) > 0) {
	    map_coordinate_to_pixel(x_max, y_min + y * delta);
	    for(int x = mapped_coordinate[0] - tic_width_horizontal/2; x < mapped_coordinate[0]; x++) {
		set_pixel_RGB(mapped_coordinate[1]-1, x, COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_BLUE, COORDINATE_SYSTEM_GREEN);
	    }
	}
	else {
	    map_coordinate_to_pixel(0, y_min + y * delta);
	    for(int x = mapped_coordinate[0] - tic_width_horizontal/2; x < mapped_coordinate[0] + tic_width_horizontal/2 ; x++) {
	        set_pixel_RGB(mapped_coordinate[1]-1, x, COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_BLUE, COORDINATE_SYSTEM_GREEN);
	    }
	}
    }
    //x-axis tics
    for(int x = 0; x < number_of_tics_x_axis; x++) {
	if((boundary_flags & 0b1000) > 0) {
	    map_coordinate_to_pixel(x_min + x * delta, y_min);
	    for(int y = mapped_coordinate[1]-tic_width_vertical/2; y < mapped_coordinate[1]; y++) {
		set_pixel_RGB(y, mapped_coordinate[0], COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_BLUE, COORDINATE_SYSTEM_GREEN);
	    }
	}
	else if((boundary_flags & 0b0100) > 0) {
	    map_coordinate_to_pixel(x_min + x * delta, y_max);
	    for(int y = mapped_coordinate[1]; y < mapped_coordinate[1] + tic_width_vertical; y++) {
		set_pixel_RGB(y, mapped_coordinate[0], COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_BLUE, COORDINATE_SYSTEM_GREEN);
	    }
	}
	else {
	    map_coordinate_to_pixel(x_min + x * delta, 0);
	    for(int y = mapped_coordinate[1] - tic_width_vertical/2; y < mapped_coordinate[1] + tic_width_vertical/2; y++) {
	        set_pixel_RGB(y, mapped_coordinate[0], COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_BLUE, COORDINATE_SYSTEM_GREEN);
	    }
	}

    }

    return 0;
}

/**
 * Always call this function before draw_tic_marks(), particularly if the origin is not in bounds of the plot.
 * This function draws axis based on initialized mins and maxes. If mins and maxes are restricted to less than 4 quadrants,
 * axis will appear on edges
 */
void draw_axis() {

    int coordinate_to_be_mapped_x = 0; int coordinate_to_be_mapped_y = 0;   //normally, coordinate to be mapped is origin

    //sets axis on borders if coordinate system is limited to certain quadrants. See definition of boundary_flags for description
    if(y_min >= 0) {
	x_axis_row = HEIGHT - 1;
	boundary_flags |= 0b1000;
	coordinate_to_be_mapped_y = y_min;
    }
    if(y_max <= 0) {
	x_axis_row = 0;
	boundary_flags |= 0b0100;
	coordinate_to_be_mapped_y = y_max;
    }
    if(x_min >= 0) {
	y_axis_column = 0;
	boundary_flags |= 0b0010;
	coordinate_to_be_mapped_x = x_min;
    }
    if(x_max <= 0) {
	y_axis_column = WIDTH-1;
	boundary_flags |= 0b0001;
	coordinate_to_be_mapped_x = x_max;
    }


    map_coordinate_to_pixel(coordinate_to_be_mapped_x, coordinate_to_be_mapped_y);
    if(x_axis_row == -1) {
	x_axis_row = mapped_coordinate[1];
    }
    if(y_axis_column == -1) {
	y_axis_column = mapped_coordinate[0];
    } 

    for(int x = 0; x < WIDTH; x++){
	set_pixel_RGB(x_axis_row, x, COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_GREEN, COORDINATE_SYSTEM_BLUE);
    }
    for(int y = 0; y < HEIGHT; y++) {
	set_pixel_RGB(y, y_axis_column, COORDINATE_SYSTEM_RED, COORDINATE_SYSTEM_GREEN, COORDINATE_SYSTEM_BLUE);
    }

}


/**
 * @param position_x index of mass in image array
 * @param position_y index of mass in image array
 * @return 0 if successful
 * @return -1 if drawn outside range
 */
int8_t draw_point_mass_by_pixel(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int position_x, int position_y) {
	if (!(position_x < 0 || position_x > WIDTH-1 || position_y < 0 || position_y > HEIGHT-1)) {
		set_pixel_RGB(position_y, position_x, RED, GREEN, BLUE);
		return 0;
	} 
	else {
		return -1;
	}
}

/**
 * maps position_x and y and calls draw_point_mass by pixel
 * @param RED 8 bit RGB
 * @param BLUE 8 bit RGB
 * @param GREEN 8 bit RGB
 * @param position_x position of mass in coordinate system
 * @param position_y position of mass in coordinate system
 * 
 * @return 0 if successful
 * @return -1 if outside range, mass is not drawn
 */
int8_t draw_point_mass_by_coordinate(uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y) {
	map_coordinate_to_pixel(position_x, position_y);
	return draw_point_mass_by_pixel(RED, GREEN, BLUE, mapped_coordinate[0], mapped_coordinate[1]);
}


/**
 * iterates through square drawn out by radius and draws if (x-x1)^2 + (y-y1)^2 < radius
 * uses draw_point_mass_by_pixel()
 */
int8_t draw_large_mass(uint8_t radius, uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y) {
    map_coordinate_to_pixel(position_x, position_y);
    int mapped_x = mapped_coordinate[0];
    int mapped_y = mapped_coordinate[1];

    uint16_t radius_squared = radius * radius;
    int error = 0;
    for(int i = mapped_x - radius; i < mapped_x + radius; i++){
	for(int j = mapped_y - radius; j < mapped_y + radius; j++) {
	    if( (mapped_x - i)*(mapped_x - i) + (mapped_y - j)*(mapped_y - j) <= radius_squared) {
		error = draw_point_mass_by_pixel(RED, GREEN, BLUE, i, j);
		if(error) {
		    //printf("error drawing large mass");
		}
	    }
	}
    }
    return 0;
}








