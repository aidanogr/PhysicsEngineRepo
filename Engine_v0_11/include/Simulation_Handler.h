//
// Created by Aidan O'Grady on 3/12/25.
//

#ifndef SIMULATION_HANDLER_H
#define SIMULATION_HANDLER_H

#include "UniverseHandler.h"

typedef struct{

}Simulation_Tracker;

Simulation_Tracker init_simulation_tracker(double sample_time, int number_of_samples, Universe* universe);
//int s
#endif //SIMULATION_HANDLER_H
