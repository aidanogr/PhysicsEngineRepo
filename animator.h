#ifndef ANIMATOR_H
#define ANIMATOR_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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



uint8_t open_ppm_sim(char* filename);
uint8_t initialize_animation(int64_t index_of_focused_mass);

uint8_t set_bounds(double min_x, double min_y, double max_x, double max_y);
void close_ppm_sim();
int simulate();



int8_t read_next_timestamp(simulated_mass_t* masses_at_timestamp);
//debug function
void print_simulated_mass(const simulated_mass_t *mass); 


#endif
