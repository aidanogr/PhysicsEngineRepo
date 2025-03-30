#include "frame_drawer.h"
#include "stdio.h"
#include "stdlib.h"
#include <assert.h>
#include "animator.h"


void test_coordinate_systems(int x_min_temp, int y_min_temp, int x_max_temp, int y_max_temp, int tic_delta, int counter) {
    
    initialize_frame(x_min_temp, y_min_temp, x_max_temp, y_max_temp, 0); 
    change_background_color(0, 0, 0);
    draw_axis();
    draw_tic_marks(4,4, tic_delta);

    char filename[100];
    sprintf(filename, "%s%d%s","./tested_coordinate_systems/coordinate_system_", counter, ".ppm");
    printf("%s", filename);
    save_ppm(filename, image);

}

void test_multiple_coordinate_systems() {
    test_coordinate_systems(-10, -10, 10, 10, 1, 0); 
    test_coordinate_systems(-10, 1, 10, 10, 1, 1);
    test_coordinate_systems(1, 1, 10, 10, 1, 2);
    test_coordinate_systems(1, -10, 10, 10, 1, 3);
    test_coordinate_systems(-10, -10, -4, -4, 1, 4);
    test_coordinate_systems(-10, -10, -4, 4, 1, 5);    
    test_coordinate_systems(-500, -500, -200, 150, 20, 6);
}


int main() {
    int err = open_ppm_sim("/Users/aidanogrady/Library/CloudStorage/OneDrive-Personal/Documents/Computer Science/C/PhysicsEngineRepo/Engine_v0_11/Simulations/Earth_Human_Gravity.psim");
   if(err != 0) {
	printf("error opening sim file: code %d\n", err);
	return -1;
    }

    //set_bounds(6377744.612457, -10, 6378238.000000, 10);
    err = initialize_animation(0);
    if(err != 0) {
	printf("error initializing animation: code %d\n", err);
    }
    if(err != 0) return -2;
    simulate();

    //printf("number of masses: %d\n", number_of_masses);
/*    x_min = -10;
    y_min = -10;
    x_max = 10;
    y_max = 10;
    //TODO check for valid bounds 
    
    
    initialize_frame(x_min, y_min, x_max, y_max, 1); 
    change_background_color(0, 0, 0);
    draw_axis();
    draw_tic_marks(4,4, 1);
    draw_point_mass_by_coordinate(200, 100, 50, 1, 1);
    draw_large_mass(4, 200, 0, 0, 2.2, 4.3);
    save_ppm("coordinate_system.ppm", image);
    test_multiple_coordinate_systems();
    */

} 







