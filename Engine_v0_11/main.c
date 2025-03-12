//
// Created by Aidan O'Grady on 3/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "UNIVERSAL_CONSTANTS.h"
#include "UniverseHandler.h"
#include "Animate.h"



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
    //DELTA_TIME_SECONDS = 0.01;
    printf("DEltaTimeSeconds: %f\n", DELTA_TIME_SECONDS);
    const float time_of_simulation_seconds = 10;
    const int number_of_iterations = (int) (time_of_simulation_seconds / DELTA_TIME_SECONDS);

    Universe universe;
    createUniverse(&universe);
    Mass m2 = createMass(EARTH_MASS_KG, 0, 0, 0, 0, 0, 0, 0);
    Mass m1 = createMass(70, 0, EARTH_RADIUS_M + 100, 0, 0, 0, 0, 0);
    if (addMassToUniverse(&universe, &m1) != 0 || addMassToUniverse(&universe, &m2) != 0) {
        return -1;
    }
    printMasses(&universe);

    int x_min = -1000; int y_min = -1000;
    int x_max = 1000; int y_max = 1000;
    init_animation_2d(x_min, y_min, x_max, y_max);

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
        counter++;
    }

    printf("\n\n");
    printMasses(&universe);
    return 1;

}