//
// Created by Aidan O'Grady on 3/12/25.
//

#include "../include/UniverseHandler.h"
#include <stdio.h>

/*
typedef enum {
    TEST_POSITION, TEST_VELOCITY
};
*/

int test_set_DELTA_TIME_SECONDS(double delta_time, int shouldReturn) {
    int result = set_DELTA_TIME_SECONDS(delta_time);
    if(result != shouldReturn) {
      printf("Error in set_DELTA_TIME_SECONDS: returned %d, should return %d\n", result, shouldReturn);
      return -1;
    }
    if(shouldReturn == 0) {
      if(*DELTA_TIME_SECONDS_POINTER != delta_time) {
        printf("DELTA_TIME_SECONDS_POINTER does not equal DELTA_TIME.\n");
        return -1;
      }
    }
    return 1;
}

int test_createMass(double mass_kg, double charge, double initial_position_m_x, double initial_position_m_y, double initial_position_m_z,
    double initial_velocity_m_per_s_x, double initial_velocity_m_per_s_y, double initial_velocity_m_per_s_z) {
    return -1;
}

int test_addMassToUniverse(Universe* universe, Mass* mass) {
    return -1;
}

int test_createUniverse(Universe* universe) {
    return -1;
}

int test_distance_between_masses_squared(Mass* first_mass, Mass* second_mass, Vector* vector_between_masses) {
    return -1;
}

int test_normalizeVector(Vector* vector) {
    return -1;
}

int test_applyGravitationalForce(Universe* universe) {
    return -1;
}

int test_applyElectrostaticForce(Universe* universe) {
    return -1;
}

int test_scenario_earth_and_human(Universe* universe) {
    return -1;
}

int main() {
    int number_failed_tests = 0;

    //SET DELTA TIME TESTS
    if(test_set_DELTA_TIME_SECONDS(10.0, 0) != 1)   {number_failed_tests++;}
    if(test_set_DELTA_TIME_SECONDS(-15.0, -1) != 1) {number_failed_tests++;}
    if(test_set_DELTA_TIME_SECONDS(0.0, -1) != 1)   {number_failed_tests++;}

    printf("number_failed_tests: %d\n", number_failed_tests);
}
