#include "animator.h"
#include "frame_drawer.h"
#include <stdlib.h>

//wow this is really bad and someone should probably do something about that lol
#define SIZE_OF_MASS_STAMP 640

FILE* sim_file;

double bounds[4] = {0};

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
    bounds[0] = min_x;
    bounds[1] = min_y;
    bounds[2] = max_x;
    bounds[3] = max_y;
    return 0;
}


/**
 * sets bounds automatically by focusing the window of animation on 1 mass 
 */
uint8_t calculate_bounds_about_mass(int64_t index_of_focused_mass) {
    //TODO check this check and make it an assert, ensures number_of_masses has been read
    printf("%d", ftell(sim_file) == sizeof(int)); 
 
    fseek(sim_file, index_of_focused_mass*(SIZE_OF_MASS_STAMP), SEEK_CUR);
    fseek(sim_file, sizeof(uint64_t) + sizeof(double) * 3, SEEK_CUR);
    double position_x_min_buffer = 0; 
    double position_y_min_buffer = 0;
    double position_x_max_buffer = 0;
    double position_y_max_buffer = 0;
    double max_delta_x = 0;
    double max_delta_y = 0;

    fread(&position_x_min_buffer, sizeof(double), 1, sim_file);
    fread(&position_y_min_buffer, sizeof(double), 1, sim_file);

    position_x_max_buffer = position_x_min_buffer;
    position_y_max_buffer = position_y_min_buffer;
    
 //   fread(&position_x_max_buffer, sizeof(double), 1, sim_file);
  //  fread(&position_y_max_buffer, sizeof(double), 1, sim_file);

    double buffer = 0;

    fseek(sim_file, number_of_masses * SIZE_OF_MASS_STAMP - sizeof(double)*2 ,SEEK_CUR);
    
    while(ftell(sim_file) < file_size) {
	//fseek(sim_file, sizeof(uint64_t) + sizeof(double)*3, SEEK_CUR);

	fread(&buffer, sizeof(double), 1, sim_file);
	if(buffer < position_x_min_buffer) position_x_min_buffer = buffer;
	if(buffer > position_x_max_buffer) position_x_max_buffer = buffer;
//	if(buffer - position_x_min_buffer > max_delta_x) {
	    //max_delta_x = buffer - position_x_min_buffer;
	//}

	fread(&buffer, sizeof(double), 1, sim_file);
	if(buffer < position_y_min_buffer) position_y_min_buffer = buffer;
	if(buffer > position_y_max_buffer) position_y_max_buffer = buffer;

	if(buffer - position_y_min_buffer > max_delta_y) {
	    max_delta_y = buffer - position_y_min_buffer;
	}

	fseek(sim_file, number_of_masses * SIZE_OF_MASS_STAMP - sizeof(double)*2, SEEK_CUR);
//	fseek(sim_file, sizeof(double) * 4, SEEK_CUR);

    }
    fseek(sim_file, sizeof(int), SEEK_SET);
    bounds[0] = position_x_min_buffer;
    bounds[1] = position_y_min_buffer;
    bounds[2] = position_x_max_buffer;
    bounds[3] = position_y_max_buffer;
    //if(max_delta_x == 0) {
//	bounds[0] = -10;
//	bounds[2] = 10;
 //   }
    if(max_delta_y == 0) {
	bounds[1] = -10;
	bounds[3] = 10;
    }

    printf("\n%.2f %.2f %.2f %.2f", bounds[0], bounds[1], bounds[2], bounds[3]);

    return 0;
}


/**
 * crashes if sim_file is not initialized. Ensure open_ppm_sim() is called
 *
 * @param index_of_focused_mass : pass negative 1 to manually set bounds using set_bounds(), call set_bounds() AFTER this function.
 * index_of_focused_mass determines the minimum and maximum bounds of the animation by finding the minimum and maximum bounds in the psim file.
 * If this is not ideal, manully set with set_bounds and calculate bounds some other way.
 */
uint8_t initialize_animation(int64_t index_of_focused_mass) {
    assert(sim_file != NULL);

    fseek(sim_file, 0, SEEK_END);
    file_size =  ftell(sim_file);
    fseek(sim_file, 0, SEEK_SET);

    //TODO check for errors here, EXTREMELY IMPORTANT. ALSO MAKE number_of_masses uint64_t OR SOMETHING
    fread(&number_of_masses, sizeof(int), 1, sim_file);
    
    assert(index_of_focused_mass >= -1 && index_of_focused_mass < number_of_masses);
    if(index_of_focused_mass != -1) {
	calculate_bounds_about_mass(index_of_focused_mass);
    }

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

uint8_t convert_to_mp4() {
 //   ffmpeg -framerate 10 -i int_file_%d.ppm -c:v libx264 -crf 25 -vf "scale=500:500,format=yuv420p" -movflags +faststart output.mp4
    system("ffmpeg -framerate 10 -i \"/Users/aidanogrady/OneDrive/Documents/Computer Science/C/AnimationLibrary/animation_files/int_file_%d.ppm\" -c:v libx264 -crf 25 -vf \"scale=500:500,format=yuv420p\" -movflags +faststart -y output.mp4");
    return 0;
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
	
	//initialize_frame(6377744.612457, -10, 6378238.000000, 10, number_of_masses);
	initialize_frame(bounds[0], bounds[1], bounds[2], bounds[3], number_of_masses);
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
    convert_to_mp4();
    return 0;
}

