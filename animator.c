#include "animator.h"


FILE* sim_file;



long int file_size = 0;

/*
 * Must always call this function before using any other animation function calls
 * ALWAYS CALL close_ppm_sim(); After use  
 */
uint8_t open_ppm_sim(char* filename) {
    if(strnlen(filename, 256) > 255) {
	return -1;
    }
    
    sim_file = fopen(filename, "rb");
    if(sim_file == NULL) {
	return -2;
    }


    return 0;
}


/**
 * ONLY USE IF FOCUSED_MASS IS SET TO -1 from: initialize_animation(int64_t index_of_focused_mass);
 * this function should be called AFTER initialize_animation
 */
uint8_t set_bounds(double min_x, double min_y, double max_x, double max_y) {
    
}

/**
 * crashes if sim_file is not initialized
 * @param index_of_focused_mass : pass negative 1 to manually set bounds using set_bounds(), call set_bounds() AFTER this function.
 */
uint8_t initialize_animation(int64_t index_of_focused_mass) {
    assert(sim_file != NULL);

    fseek(sim_file, 0, SEEK_END);
    file_size =  ftell(sim_file);
    fseek(sim_file, 0, SEEK_SET);

    //TODO check for errors here, EXTREMELY IMPORTANT
    fread(&number_of_masses, sizeof(int), 1, sim_file);

    return 0;
}

int8_t read_next_timestamp(simulated_mass_t* masses_at_timestamp) {

    if(ftell(sim_file) >= file_size) {
	return -1;
    }
    for(int i = 0; i < number_of_masses; i++) {
	fread(&masses_at_timestamp[i].timestamp, sizeof(double), 1, sim_file); 
        fread(&masses_at_timestamp[i].index, sizeof(uint64_t), 1, sim_file);
        fread(&masses_at_timestamp[i].masses_kg, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].charges, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].pxs, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].pys, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].pzs, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].vxs, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].vys, sizeof(double), 1, sim_file);
        fread(&masses_at_timestamp[i].vzs, sizeof(double), 1, sim_file);
    }
	return 0;
}

/**
 *  DO NOT CALL THIS FUNCTION BEFORE sim_file IS OPENED WITH open_ppm_sim();
 *  Will intentionally crash the program 
 */
void close_ppm_sim() {
   int err = fclose(sim_file);
   assert(err == 0);
}


void print_simulated_mass(const simulated_mass_t *mass) {
    printf("Simulated Mass Data:\n");
    printf("  Timestamp: %.6f\n", mass->timestamp);
    printf("  Index: %llu\n", mass->index);
    printf("  Mass (kg): %.6f\n", mass->masses_kg);
    printf("  Charge: %.6f\n", mass->charges);
    printf("  Position (px, py, pz): (%.6f, %.6f, %.6f)\n", mass->pxs, mass->pys, mass->pzs);
    printf("  Velocity (vx, vy, vz): (%.6f, %.6f, %.6f)\n", mass->vxs, mass->vys, mass->vzs);
}

int simulate() {

    simulated_mass_t masses_at_timestamp[number_of_masses];
    int counter = 0;
    char filename[256];

    while(1) {
	int err	= read_next_timestamp(masses_at_timestamp);
	if(err == -1) {
	    break;
	}
	
	initialize_frame(6377744.612457, -10, 6378238.000000, 10, number_of_masses);
	draw_tic_marks(4, 4, 10);
	


	for(int i = 0; i < number_of_masses; i++) {
	   // print_simulated_mass(&(masses_at_timestamp[i]));
	    draw_large_mass(10, 200, 200, 0, masses_at_timestamp[i].pxs, masses_at_timestamp[i].pys);
	}
	sprintf(filename, "/Users/aidanogrady/OneDrive/Documents/Computer Science/C/AnimationLibrary/animation_files/int_file_%d.ppm", counter);
	save_ppm(filename, image);
	counter++;
    }
    /*
    uint64_t index = 0;
    double double_buffer = 0;
    fread(&double_buffer, sizeof(double), 1, sim_file);
    printf("%.3f", double_buffer);
    fread(&index, sizeof(uint64_t), 1, sim_file);
    printf("%llu\n", index);


    fread(&double_buffer, sizeof(double), 1, sim_file);
    printf("%.3f", double_buffer);   
    */
    close_ppm_sim();

}

