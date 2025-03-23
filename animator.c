#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <_strings.h>
#include "animator.h"





int number_of_masses = 0;

double x_min;
double y_min;
double x_max;
double y_max;

uint8_t image[HEIGHT][WIDTH][3] = {0};
double delta_width;
double delta_height;

int mapped_coordinate[] = {0, 0};

//-1 meaning "unset" (see draw_axis())
int x_axis_row = -1;
int y_axis_column = -1;



/**
 * MUST CALL THIS FUNCTION BEFORE USING ANY OTHER   
 */
void initialize_animation(double x_min_temp, double y_min_temp, double x_max_temp, double y_max_temp, int number_of_masses) {
    x_min = x_min_temp;
    y_min = y_min_temp;
    x_max = x_max_temp;
    y_max = y_max_temp;
    delta_width = (x_max-x_min)/WIDTH;
    delta_height = (y_max-y_min)/HEIGHT;
}


/**
 * @return 0 if file saved successfully
 * @return -1 if unsuccessful
 */
int save_ppm(const char *filename, uint8_t image[HEIGHT][WIDTH][3]) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Failed to open file for writing.\n");
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, 1, WIDTH * HEIGHT * 3, fp);
    fclose(fp);
    return 0;
}


/**
 * updates mapped_coordinate[x,y]
 * @param coordinate_x
 * @param coordinate_y
 *
 * @return 0 if successful
 * @return -1 if unsuccessful, mapped_coordinate
 */
int map_coordinate_to_pixel(double coordinate_x, double coordinate_y) {
    if(coordinate_y < y_min || coordinate_y > y_max || coordinate_x < x_min || coordinate_x > x_max) {
	printf("Tried to map coordinate outside of range");
  	return -1;
    }
    mapped_coordinate[0] = (int) ((coordinate_x - x_min) / delta_width);
    mapped_coordinate[1] = HEIGHT - (int) ((coordinate_y - y_min) / delta_height);
    return 0;
}


/**
 * note: tic width and height represent lengths of tic either horizontal (width) 
 *	 or vertical (height), these are not the dimesions of a single tic. By default, thickness of 
 *	 tic is 1 pixel. //TODO add variable tic widths
 * @param tic_width_percentage : width of tic expressed as percentage of width
 * @param tic_height_percentage : height of tic expressed as percentage of height
 * @param number_of_tics_per_axis : 
*/
int draw_tic_marks(int tic_width_percentage, int tic_height_percentage, double delta) {

    int tic_width = (int) (0.01 * tic_width_percentage * WIDTH);
    int tic_height = (int) (0.01 * tic_height_percentage * HEIGHT);


    for(int y = y_min; y < (int) ((y_max-y_min)/delta) ; y++) {
	map_coordinate_to_pixel(0, y_min + y * delta);
	for(int x = mapped_coordinate[0] - tic_width; x < mapped_coordinate[0] + tic_width; x++) {
	      image[mapped_coordinate[1]][x][0] = 200;
	      image[mapped_coordinate[1]][x][1] = 0;
	      image[mapped_coordinate[1]][x][2] = 0;
	}
    }
    for(int x = 0; x < (int) (x_max-x_min)/delta; x++) {
	map_coordinate_to_pixel(x_min + x * delta,0);
	for(int y = mapped_coordinate[1] - tic_height; y < mapped_coordinate[1] + tic_height; y++) {
	    image[y][mapped_coordinate[0]][0] = 200;
	    image[y][mapped_coordinate[0]][1] = 0;
	    image[y][mapped_coordinate[0]][2] = 0;

	}
    }
    return 0;
}


void draw_axis() {

    //sets axis on borders if coordinate system is limited to certain quadrants
    if(y_min > 0) {
	x_axis_row = HEIGHT - 1;
	printf("gate 1");
    }
    if(y_max < 0) {
	x_axis_row = 0;
 	printf("gate 2");
   }
    if(x_min > 0) {
	y_axis_column = 0;
 	printf("gate 3");
   }
    if(x_max < 0) {
    	y_axis_column = WIDTH-1;
 	printf("gate 4");
   }
    
    //otherwise place them according to their mapping from the orgin
    map_coordinate_to_pixel(0,0);
    if(x_axis_row == -1) {
	x_axis_row = mapped_coordinate[1];
    }
    if(y_axis_column == -1) {
	y_axis_column = mapped_coordinate[0];
    } 
 
    for(int x = 0; x < WIDTH; x++){
    	image[x_axis_row][x][0] = 255; 
    }
    for(int y = 0; y < HEIGHT; y++) {
	image[y][y_axis_column][0] = 255;
    }

}


/**
 * @param position_x index of mass in image array
 * @param position_y index of mass in image array
 * @return 0 if successful
 * @return -1 if drawn outside range
 */
int draw_point_mass_by_pixel(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int position_x, int position_y) {
	if (!(position_x < 0 || position_x > WIDTH-1 || position_y < 0 || position_y > HEIGHT-1)) {
		image[position_y][position_x][0] = RED;
		image[position_y][position_x][1] = GREEN;
		image[position_y][position_x][2] = BLUE;
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
int draw_point_mass_by_coordinate(uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y) {
	map_coordinate_to_pixel(position_x, position_y);
	return draw_point_mass_by_pixel(RED, GREEN, BLUE, mapped_coordinate[0], mapped_coordinate[1]);
}


/**
 * iterates through square drawn out by radius and draws if (x-x1)^2 + (y-y1)^2 < radius
 * uses draw_point_mass_by_pixel()
 */
int draw_large_mass(uint8_t radius, uint8_t RED, uint8_t GREEN, uint8_t BLUE, double position_x, double position_y) {
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
		    printf("error drawing large mass");
		}
	    }
	}
    }
    return 0;
}



//	draw_large_mass()

	//FILE* psimFile = fopen("simulation_files/Earth_Human_Gravity.psim", "wb");
	//uint64_t number_of_masses = 0;
	//fread(number_of_masses, sizeof(uint64_t), 1, psimFile);

