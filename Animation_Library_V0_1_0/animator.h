#ifndef ANIMATOR_H
#define ANIMATOR_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "frame_drawer.h"


typedef struct {
    double timestamp;
    uint64_t index;
    double masses_kg;
    double charges;
    double pxs;
    double pys;
    double pzs;
    double vxs;
    double vys;
    double vzs;

} simulated_mass_t;



int8_t open_ppm_sim(char* filename);

int8_t initialize_animation(char* ppm_sim_path, char* _output_file_path, char* _output_file_dir, int64_t index_of_focused_mass);

int8_t set_bounds(double min_x, double min_y, double max_x, double max_y);
void close_ppm_sim();
int8_t animate();



int8_t read_next_timestamp(simulated_mass_t* masses_at_timestamp);
//debug function
void print_simulated_mass(const simulated_mass_t *mass); 


#endif
