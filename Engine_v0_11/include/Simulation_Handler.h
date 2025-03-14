//
// Created by Aidan O'Grady on 3/12/25.
//

#ifndef SIMULATION_HANDLER_H
#define SIMULATION_HANDLER_H

#include "UniverseHandler.h"

typedef struct{
    Universe* universe;
    double time_stamp_seconds;
    FILE* tracker_file;
    size_t bytes_written; //initialized here to avoid headache during write checks
}Simulation_Tracker;


int init_simulation_tracker(char* path, Universe* universe, Simulation_Tracker* tracker);

int tracker_update(double timestamp,Simulation_Tracker* tracker);

#endif //SIMULATION_HANDLER_H
