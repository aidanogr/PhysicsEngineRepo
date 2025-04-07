#include "animator.h"


int8_t string_contains(char* str, char* substring) {
    uint8_t counter = 0;
    int8_t consecutive_characters_correct = 0;
    while(counter < 255 && consecutive_characters_correct < strnlen(substring, 255) && counter < strnlen(str, 255)) {
	if(str[counter] == substring[consecutive_characters_correct]) {
	    consecutive_characters_correct++;
	}
    }
    if(consecutive_characters_correct == strnlen(substring, 255)) { return 1; }
    return -1;

}


/**
 * Usage: physics_animate -f (simulation file path)
 * < -bounds (x_min, y_min, x_max, y_max) or -focus_mass (focused masses index) >
 * < -o (output file path) > 
 * < -k (no additional paramaters, use flag to keep intermediate files)
 * 
 */
int main(int argc, char** argv) {
    const char* usage_string = 
      "Usage: physics_animate -f (simulation file path)\n"
      "< -bounds (x_min,y_min,x_max,y_max) or -focus_mass (focused masses index) >\n"
      "< -o (output file path) >\n"
      "< -k (no additional paramaters, use flag to keep intermediate files)\n" 
      "example: physics_animate -f ./sample.psim -bounds -1,-10,5,6 -o output.mp4\n";

    if(argc < 3) { printf("Error, No flags set.\n%s", usage_string); } 
    
    //search for set bounds
    for(int i = 0; i < argc; i++) {
	if(!strcmp(argv[i], "-bounds")) {
	    if(i + 1 >= argc) { printf("Bad usage:\n%s,usage_string"); }
	    char* bounds = argv[i+1];
	    if(string_contains(bounds, ",")) {
		for(int j = 0; j < strnlen(bounds); j++) {
		       
		}
	    }





    int err = open_ppm_sim("/Users/aidanogrady/Documents/GitHub/PhysicsEngineRepo/Physics_Engine_v0_0_1/Simulations/Earth_Human_Gravity.psim");
    if(err != 0) {
	printf("error opening sim file: code %d\n", err);
	return -1;
    }

    //    set_bounds(6377744.612457, -10, 6378238.000000, 10);
    err = initialize_animation(0);
    if(err != 0) {
	printf("error initializing animation: code %d\n", err);
    }
    if(err != 0) return -2;
    animate();



    
    return 0;
}
