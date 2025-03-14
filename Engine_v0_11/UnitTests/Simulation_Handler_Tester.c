//
// Created by Aidan O'Grady on 3/13/25.
//

#include <stdio.h>
#include "../include/Simulation_Handler.h"
typedef struct {
    double vx, vy, vz;
} Vector3D;
typedef struct {
    int index;
    double mass_kg;
    double charge;
    Vector3D position_meters;
    Vector3D velocity_meters;
} MassData;


void testCasting() {
    double deltaTime=  0.1;
    int iterations_per_second = (int) (1 / deltaTime);
    printf("%f -> %d", deltaTime, iterations_per_second );


    deltaTime=  0.15;
    iterations_per_second = (int) (1 / deltaTime);
    printf("%f -> %d", deltaTime, iterations_per_second );

}


void testFileRead() {

    FILE* file = fopen("/Users/aidanogrady/Library/CloudStorage/OneDrive-Personal/Documents/Computer Science/C/PhysicsEngineRepo/Engine_v0_11/Simulations/Earth_Human_Gravity.psim", "rb");
    double time_stamp;
    MassData buffer_mass;

    while (fread(&time_stamp, sizeof(double), 1, file) == 1) {
        fread(&buffer_mass.index, sizeof(int), 1, file);
        fread(&buffer_mass.mass_kg, sizeof(double), 1, file);
        fread(&buffer_mass.charge, sizeof(double), 1, file);
        fread(&buffer_mass.position_meters.vx, sizeof(double), 1, file);
        fread(&buffer_mass.position_meters.vy, sizeof(double), 1, file);
        fread(&buffer_mass.position_meters.vz, sizeof(double), 1, file);
        fread(&buffer_mass.velocity_meters.vx, sizeof(double), 1, file);
        fread(&buffer_mass.velocity_meters.vy, sizeof(double), 1, file);
        fread(&buffer_mass.velocity_meters.vz, sizeof(double), 1, file);

        printf("%lf,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
               time_stamp, buffer_mass.index, buffer_mass.mass_kg, buffer_mass.charge,
               buffer_mass.position_meters.vx, buffer_mass.position_meters.vy, buffer_mass.position_meters.vz,
               buffer_mass.velocity_meters.vx, buffer_mass.velocity_meters.vy, buffer_mass.velocity_meters.vz);
    }

    fclose(file);
}