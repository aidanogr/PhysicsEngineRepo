#include "animator.h"
#include "frame_drawer.h"

//wow this is really large, someone should probably do something about that lol
//

#define SIZE_OF_MASS_STAMP_BYTES sizeof(simulated_mass_t)
#define POSITION_OFFSET_BYTES sizeof(uint64_t) + sizeof(double)*3
#define SIZE_OF_POSITIONS_2D_BYTES sizeof(double) * 2


FILE* sim_file;
char* output_file_path;
//used for intermediate files
char* output_file_dir;

double bounds[4] = {0};
long int file_size = 0;
int framerate = 0;
int timestamps_per_second = 0;


/*
 * Must always call this function before using any other animation function calls
 * ALWAYS CALL close_ppm_sim(); After use  
 */
int8_t open_ppm_sim(char* filename) {
    if(strnlen(filename, 256) >= 255) {
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
int8_t set_bounds(double min_x, double min_y, double max_x, double max_y) {
    bounds[0] = min_x;
    bounds[1] = min_y;
    bounds[2] = max_x;
    bounds[3] = max_y;
    return 0;
}


//TODO this function is getting a little long; either make it more concise or break it up
/**
 * sets bounds automatically by focusing the window of animation on 1 mass 
 */
uint8_t calculate_bounds_about_mass(int64_t index_of_focused_mass) {
    //TODO check this check and make it an assert, ensures number_of_masses has been read
    assert(ftell(sim_file) == sizeof(int)); 
 
    //goto start of indexed mass
    fseek(sim_file, index_of_focused_mass*(SIZE_OF_MASS_STAMP_BYTES) + POSITION_OFFSET_BYTES, SEEK_CUR);

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
 
    //printf("\n%.2f,%.2f,%.2f,%.2f\n", position_x_min_buffer, position_y_min_buffer, position_x_max_buffer, position_y_max_buffer);
    double buffer = 0;
    int counter= 0;
    fseek(sim_file, number_of_masses * SIZE_OF_MASS_STAMP_BYTES - SIZE_OF_POSITIONS_2D_BYTES ,SEEK_CUR);
    
    while(fread(&buffer, sizeof(double), 1, sim_file) == 1) {

	if(fabs(buffer - position_x_min_buffer) > max_delta_x) {
	    max_delta_x = buffer - position_x_min_buffer;
	}
	if(buffer < position_x_min_buffer){
	    position_x_min_buffer = buffer;
	}
	if(buffer > position_x_max_buffer) {
	    position_x_max_buffer = buffer;
	}
	//printf("\n%.2f\n", buffer);

	fread(&buffer, sizeof(double), 1, sim_file);
	if(fabs(buffer - position_y_min_buffer) > max_delta_y) {
	    max_delta_y = buffer - position_y_min_buffer;
	}
	if(buffer < position_y_min_buffer){
	    position_y_min_buffer = buffer; 
	}
	if(buffer > position_y_max_buffer){
	    position_y_max_buffer = buffer;
	}
	//printf("\n%.2f\n", buffer);
	fseek(sim_file, number_of_masses * SIZE_OF_MASS_STAMP_BYTES - SIZE_OF_POSITIONS_2D_BYTES, SEEK_CUR);
	counter ++;
    }
    //printf("\nCounts during bounds calculation: %d", counter);

    bounds[0] = position_x_min_buffer;
    bounds[1] = position_y_min_buffer;
    bounds[2] = position_x_max_buffer;
    bounds[3] = position_y_max_buffer;


    //if bounds on an axis are zero, set arbitrary bounds
    if(max_delta_x == 0) {
	bounds[0] = -10;
	bounds[2] = 10;
    }
    if(max_delta_y == 0) {
	bounds[1] = -10;
	bounds[3] = 10;
    }


    fseek(sim_file, sizeof(int), SEEK_SET);


//   printf("\n%.2f %.2f %.2f %.2f", bounds[0], bounds[1], bounds[2], bounds[3]);
//
//reset file pointer to start of file, shifted by header
//file pointer might go past EOF which is okay, but we need to reset it if that's the case
//    clearerr(sim_file);


    return 0;
}


/**
 * crashes if sim_file is not initialized. Ensure open_ppm_sim() is called
 *
 * @param index_of_focused_mass : pass negative 1 to manually set bounds using set_bounds(), call set_bounds() AFTER this function.
 * index_of_focused_mass determines the minimum and maximum bounds of the animation by finding the minimum and maximum bounds in the psim file.
 * If this is not ideal, manully set with set_bounds and calculate bounds some other way.
 */
int8_t initialize_animation(char* ppm_sim_path, char* _output_file_dir, char* _output_file_path, int64_t index_of_focused_mass) {
    if(open_ppm_sim(ppm_sim_path) != 0) {
	printf("error opening ppm from path %s. exiting...", ppm_sim_path);
	return -1;
    }
    output_file_path = _output_file_path;
    output_file_dir = _output_file_dir; 

    fseek(sim_file, 0, SEEK_END);
    
    file_size =  ftell(sim_file);
    fseek(sim_file, 0, SEEK_SET);
    
    //TODO check for errors here, EXTREMELY IMPORTANT. ALSO MAKE number_of_masses uint64_t OR SOMETHING
    fread(&number_of_masses, sizeof(int), 1, sim_file);

    
    //printf("%lld, %d", index_of_focused_mass, number_of_masses);
    assert(index_of_focused_mass >= -1);
    assert(index_of_focused_mass < number_of_masses);

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

int8_t convert_to_mp4() {
 //   ffmpeg -framerate 10 -i int_file_%d.ppm -c:v libx264 -crf 25 -vf "scale=500:500,format=yuv420p" -movflags +faststart output.mp4
    assert(framerate >= 0);
    char command[500];
    sprintf(command, "ffmpeg -framerate %d -i \"%s/int_file_%%d.ppm\" -c:v libx264 -crf 25 -vf \"scale=500:500,format=yuv420p\" -movflags +faststart -y %s", framerate, output_file_dir, output_file_path);
    if(system(command) != 0) {
	printf("\nFFMPEG FAILURE\n");
	return -1;
    }
    return 0;
}

//@param framerate : pass -1 to let framerate be same resolution as simulation
int8_t animate() {

    simulated_mass_t masses_at_timestamp[number_of_masses];
    int counter = 0;
    char filename[256];
    double last_time_stamp = 0; 
    while(1) {
	int err	= read_next_timestamp(masses_at_timestamp);
	if(err == -1) {
	    break;
	}
	
	initialize_frame(bounds[0], bounds[1], bounds[2], bounds[3], number_of_masses);
	draw_tic_marks(4, 4, 10);
	


	for(int i = 0; i < number_of_masses; i++) {
	    draw_large_mass(10, 200, 200, 0, masses_at_timestamp[i].pxs, masses_at_timestamp[i].pys);
	}
	last_time_stamp = masses_at_timestamp[0].timestamp;
	sprintf(filename, "%s/int_file_%d.ppm", output_file_dir, counter);
	err = save_ppm(filename, image);
	if(err == -1) {
	    printf("Exiting animation ...");
	    return -1;
	}
	counter++;
    }
    printf("counts during simulation: %d", counter);
    close_ppm_sim();

    framerate = counter / last_time_stamp;
    convert_to_mp4();
    return 0;
}

