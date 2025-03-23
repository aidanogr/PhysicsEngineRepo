#include "animator.h"


int main() {
    int16_t image_[HEIGHT][WIDTH][3] = {0};
    x_min = -1;
    y_min =  0;
    x_max = 4;
    y_max = 10;

    //TODO check for valid bounds 

    
    initialize_animation(x_min, y_min, x_max, y_max, 1); 
    draw_axis();
    draw_tic_marks(4,4, 1);
 //   draw_point_mass_by_coordinate(200, 100, 50, 1, 1);
 //   draw_large_mass(4, 200, 0, 0, 2.2, 4.3);
    save_ppm("coordinate_system.ppm", image);
    
  
}




