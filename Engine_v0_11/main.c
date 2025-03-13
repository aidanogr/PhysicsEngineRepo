//
// Created by Aidan O'Grady on 3/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "include/UNIVERSAL_CONSTANTS.h"
#include "include/UniverseHandler.h"
#include "include/Animate.h"
#include "include/Simulation_Handler.h"


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

int main(void) {

    //TODO I'm almost certain that theres a casting bug in here somewhere (If it ever works in the first place)
    set_DELTA_TIME_SECONDS(0.01);
    double DELTA_TIME_SECONDS = *DELTA_TIME_SECONDS_POINTER;
    const float time_length_of_simulation_seconds = 10;
    const int number_of_iterations = (int) (time_length_of_simulation_seconds / DELTA_TIME_SECONDS);
    int frame_rate = 60;
    const int iterations_per_second = (int) (1.0/DELTA_TIME_SECONDS);
    const int iterations_per_sample = (int) ((iterations_per_second/frame_rate) + 1);
    int number_of_samples = number_of_iterations/iterations_per_sample;

    Universe universe;
    createUniverse(&universe);
    Mass m2 = createMass(EARTH_MASS_KG, 0, 0, 0, 0, 0, 0, 0);
    Mass m1 = createMass(70, 0, EARTH_RADIUS_M + 100, 0, 0, 0, 0, 0);
    if (addMassToUniverse(&universe, &m1) != 0 || addMassToUniverse(&universe, &m2) != 0) {
        return -1;
    }
    printMasses(&universe);

    Simulation_Tracker simulation =
        init_simulation_tracker(DELTA_TIME_SECONDS*iterations_per_sample, number_of_samples, &universe);

    int counter = 0;
    while (counter < number_of_iterations) {
        if (applyGravitationalForce(&universe) != 0) {
            return -2;
        }
        /*if (applyElectrostaticForce(&universe) != 0) {
            return -3;
        } */
        if (applyVelocity(&universe) != 0) {
            return -4;
        }
        if (counter % iterations_per_second == 0) {

          //  if (_store() != 0) {
           //     return -5;
          //  }
        }
        counter++;
    }

    printf("\n\n");
    printMasses(&universe);
    return 1;

}