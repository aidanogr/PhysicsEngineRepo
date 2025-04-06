//
// Created by Aidan O'Grady on 3/12/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/Simulation_Handler.h"


/*int calculate_max_fps(int max_fps, int delta_time) {
    if
} */

/**
 *
 * @param sample_time_seconds
 * @param number_of_samples
 * @param path
 * @param universe
 * @return NULL if problem initializing file or simulation.
 */
int init_simulation_tracker(char* path, Universe* universe, Simulation_Tracker* sim) {

    Simulation_Tracker simulation_tracker = {universe,0,
        NULL,0};

    FILE* simulation_tracker_file = fopen(path, "wb");
    if (simulation_tracker_file == NULL)  {
        printf("Error opening simulation tracker file\n");
        return -1;
    }
    fwrite(&universe->number_of_masses, sizeof(int), 1,simulation_tracker_file);
    simulation_tracker.tracker_file = simulation_tracker_file;
    *sim = simulation_tracker;
    return 0;
}


/**
 * writes in the following order
 * timestamp, mass_index, mass_kg, charge, position_x, position_y, position_z, velocity_x, velocity_y, velocity_z
 * @param timestamp
 * @param simulation_tracker
 * @return 0 if successful
 * @return -1 if simulation_tracker is poorly initialized
 * @retunr -2 if file write was unsuccessful
 */
int tracker_update(double timestamp, Simulation_Tracker* simulation_tracker) {
    if (simulation_tracker == NULL || simulation_tracker->tracker_file == NULL) {return -1;}

    simulation_tracker->bytes_written = 0;
    Mass* buffer_mass = simulation_tracker->universe->first_mass;
    for (int i = 0; i < simulation_tracker->universe->number_of_masses; i++) {

        simulation_tracker->bytes_written += fwrite(&(timestamp), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->index), sizeof(uint64_t), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->mass_kg), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->charge), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->position_meters.vx), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->position_meters.vy), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->position_meters.vz), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->velocity_meters.vx), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->velocity_meters.vy), sizeof(double), 1, simulation_tracker->tracker_file);
        simulation_tracker->bytes_written += fwrite(&(buffer_mass->velocity_meters.vz), sizeof(double), 1, simulation_tracker->tracker_file);
        buffer_mass = buffer_mass->next_mass;

    }
    if (simulation_tracker->bytes_written != simulation_tracker->universe->number_of_masses * 10) {
        printf("Error writing to simulation file");
        return -2;
    }
    printf("Successful print");
    return 0;
}

int terminate_simulation_tracker(Simulation_Tracker* simulation_tracker) {
    if (simulation_tracker->tracker_file == NULL) {
        return -1;
    }

    fflush(simulation_tracker->tracker_file);
    int result = fclose(simulation_tracker->tracker_file);
    if (result == 0) {
        printf("Simulation terminated gracefully");
    }
    return 0;

}

//int read_psim_file(char* path)
