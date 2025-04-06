//
// Created by Aidan O'Grady on 3/14/25.
//

#include <stdio.h>
#include <stdint.h>
#include "../include/UniverseHandler.h"

#define ROW_WIDTH_BYTES (8 + 8 + 8 + 8 +8 + 8 + 8+ 8 + 8 + 8)


typedef struct{
    Mass* masses;
    uint64_t numMasses;
}time_stamp_data_t;

void drawHeader(int width, int height, FILE *fp) {
    if (fp == NULL) return;
    fprintf(fp, "P3\n%d %d\n255\n", width, height);
}

void drawFrame(int width, int height, Mass *data, int num_masses, FILE *fp) {
    if (fp == NULL) return;

    // Draw blank canvas (white background)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(fp, "255 255 255 "); // White pixel
        }
        fprintf(fp, "\n");
    }

    // Draw masses as black dots for simplicity
    for (int k = 0; k < num_masses; k++) {
        int x = (int)(data[k].position_meters.vx);
        int y = (int)(data[k].position_meters.vy);

        if (x >= 0 && x < width && y >= 0 && y < height) {
            fseek(fp, 0, SEEK_SET);
            drawHeader(width, height, fp);

            int pixel_count = 0;
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (j == x && i == y) {
                        fprintf(fp, "0 0 0 "); // Black pixel for mass
                    } else {
                        fprintf(fp, "255 255 255 "); // White pixel
                    }
                    pixel_count++;
                }
                fprintf(fp, "\n");
            }
        }
    }
}

int read_file_header(FILE* psimFile, uint64_t* num_masses, double* time_betweeen_samples) {
    fread(num_masses, sizeof(uint64_t), 1, psimFile);
    fread(&time_betweeen_samples, sizeof(double), 1, psimFile);
    return 0; //TODO check fread's
}

int shift_file_cursor(FILE* psimFile, int num_timestamps) {
    fseek(psimFile, num_timestamps * ROW_WIDTH_BYTES, SEEK_CUR);
    return 0; //TODO check fseek errs
}
int read_data_at_time(FILE* fp, Mass* buffermass) {

}

int main() {

  /**
* psim shape:
* first 8 bytes contains number of masses (uint64_t)
*
* rest are:
*       8     2    8      8      8      8       8     8      8
*     double int double double double double double double double
*     time   mass charge px     py     pz      vx    vy     vz
*/

    FILE* file = fopen("/Users/aidanogrady/Library/CloudStorage/OneDrive-Personal/Documents/Computer Science/C/PhysicsEngineRepo/Engine_v0_11/Simulations/Earth_Human_Gravity.psim", "rb");
    if (file == NULL) {
      printf("Error opening file\n");
    }
    FILE* int_file;
    char* file_path = "../intermediate_sim_files/int_";
    char path_and_name[256];

    double time_stamp;
    Mass buffer_mass;
    int counter = 0;

    sprintf(path_and_name, "%s%d.ppm", file_path, 1);
    fopen(path_and_name, "wb");

    uint64_t numMasses = 0;
    double time_between_samples = 0;
    int num_timestamps = 1;
    read_file_header(file, &numMasses, &time_between_samples);
    shift_file_cursor(file, num_timestamps);
    read_timeStamp(file, &buffer_mass);

    //draw header


    /*
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

        if(counter % 100 == 0) {
            sprintf(path_and_name, "%s%d.ppm", file_path, counter/10);
            int_file = fopen(path_and_name, "w");
            if(int_file == NULL) {
              printf("Error opening file %s\n", path_and_name);
            }
            draw_header
            fflush(int_file);
            fclose(int_file);
        }
        counter++;
        printf("%lf,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
               time_stamp, buffer_mass.index, buffer_mass.mass_kg, buffer_mass.charge,
               buffer_mass.position_meters.vx, buffer_mass.position_meters.vy, buffer_mass.position_meters.vz,
               buffer_mass.velocity_meters.vx, buffer_mass.velocity_meters.vy, buffer_mass.velocity_meters.vz);
    }
*/



    fclose(file);

}