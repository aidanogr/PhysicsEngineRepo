#include "animator.h"
#include "stdio.h"
#include "stdlib.h"

void test_coordinate_systems(int x_min_temp, int y_min_temp, int x_max_temp, int y_max_temp, int tic_delta, int counter) {
    
    initialize_animation(x_min_temp, y_min_temp, x_max_temp, y_max_temp, tic_delta); 
    change_background_color(0, 0, 0);
    draw_axis();
    draw_tic_marks(4,4, 1);

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

}

int main() {
    x_min = -10;
    y_min = -10;
    x_max = 10;
    y_max = 10;
    //TODO check for valid bounds 
    
    
    initialize_animation(x_min, y_min, x_max, y_max, 1); 
    change_background_color(0, 0, 0);
    draw_axis();
    draw_tic_marks(4,4, 1);
 //   draw_point_mass_by_coordinate(200, 100, 50, 1, 1);
 //   draw_large_mass(4, 200, 0, 0, 2.2, 4.3);
    save_ppm("coordinate_system.ppm", image);
    test_multiple_coordinate_systems();
}




