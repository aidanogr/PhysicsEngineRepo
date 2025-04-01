//
// Created by Aidan O'Grady on 3/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../include/UNIVERSAL_CONSTANTS.h"
#include "../include/UniverseHandler.h"
#include "../include/Simulation_Handler.h"


//MACROS



//DEBUG PRINTS
void printMasses(Universe* universe) {
    Mass* anchor_mass = universe->first_mass;
    printf("Mass, Charge, position_x, position_y, position_z, velocity_x, velocity_y, velocity_z\n");
    while (anchor_mass != NULL) {
        printf("%f, %f, %f, %f, %f, %f, %f, %f\n",
            anchor_mass->mass_kg, anchor_mass->charge, anchor_mass->position_meters.vx, anchor_mass->position_meters.vy,
            anchor_mass->position_meters.vz, anchor_mass->velocity_meters.vx, anchor_mass->velocity_meters.vy,
            anchor_mass->velocity_meters.vz);
        anchor_mass = anchor_mass->next_mass;
    }
}

int main() {

    set_DELTA_TIME_SECONDS(0.01);
    double DELTA_TIME_SECONDS = *DELTA_TIME_SECONDS_POINTER;
    const float time_length_of_simulation_seconds = 10;
    const int number_of_iterations = (int) (time_length_of_simulation_seconds / DELTA_TIME_SECONDS);
    const int sampling_frequency = 10;
    /*
    int frame_rate = 60;
    const int iterations_per_second = (int) (1.0/DELTA_TIME_SECONDS);
    const int iterations_per_sample = (int) ((iterations_per_second/frame_rate) + 1);
    int number_of_samples = number_of_iterations/iterations_per_sample; */





    Universe universe;
    createUniverse(&universe);
    Mass m2 = createMass(EARTH_MASS_KG, 0, 0, 0, 0, 0, 0, 0);
    Mass m1 = createMass(70, 0, EARTH_RADIUS_M + 100, 0, 0, 0, 0, 0);
    if (addMassToUniverse(&universe, &m1) != 0 || addMassToUniverse(&universe, &m2) != 0) {
        return -1;
    }
    printMasses(&universe);

   // char* filename = "Earth_Human_Gravity.psim";
    char* path = "/Users/aidanogrady/OneDrive/Documents/Computer Science/C/PhysicsEngineRepo/Engine_v0_11/Simulations/Earth_Human_Gravity.psim";
    Simulation_Tracker simulation;
    init_simulation_tracker(path, &universe, &simulation);
    double timestamp = 0;
    tracker_update(0, &simulation);

    int counter = 0;
    while (counter < number_of_iterations) {
        timestamp += DELTA_TIME_SECONDS;
        if (applyGravitationalForce(&universe) != 0) {
            return -2;
        }
        /*if (applyElectrostaticForce(&universe) != 0) {
            return -3;
        } */
        if (applyVelocity(&universe) != 0) {
            return -4;
        }
        if (counter % sampling_frequency == 0) {
            int result = tracker_update(timestamp, &simulation);
            if (result != 0) {
                printf("Error code: %d\n", result);
            }
        }
        counter++;
    }

    tracker_update(timestamp, &simulation);

    printf("\n\n");
    printMasses(&universe);
    terminate_simulation_tracker(&simulation);
    return 1;

}
