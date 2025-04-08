#include "animator.h"
#include <stdint.h>


//The function serves as the tunnel between the java CLI and C code to be called by it, 
//all data is sanatized before use of entry
int entry(char* input_file_path, char* output_file_dir, char* output_file_path, int index_of_focused_mass,
	double bounds[], int8_t keep_intermediate_files) {
    

    int err = 0;
    if(index_of_focused_mass < 0) {
	//xmin,ymin,xmax,ymax
	set_bounds(bounds[0], bounds[1], bounds[2], bounds[3]);
	err = initialize_animation(input_file_path, output_file_dir, output_file_path, -1);
	if(err != 0) {
	    printf("error initializing animation, code %d", err);
	    return -1;
	}
    }
    else {
	err = initialize_animation(input_file_path, output_file_dir, output_file_path, index_of_focused_mass);
	if(err != 0) {
	    printf("error initializing animation: code %d\n", err);
	    return -1;
	}
    }

    err = animate();
    if(err != 0) {
	printf("\nunexpected error animating simulation: code: %d", err); 
	return -1;
    }
    return 0;
}
