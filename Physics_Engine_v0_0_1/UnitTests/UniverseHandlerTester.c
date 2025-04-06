//
// Created by Aidan O'Grady on 3/12/25.
//

#include "../include/UniverseHandler.h"
#include <stdio.h>
#include "Simulation_Handler_Tester.h"
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

int test_addOneMassToUniverse(Universe* universe, Mass* mass, int shouldReturn) {
    int result = addMassToUniverse(universe, mass);
    if(result != shouldReturn) {
        printf("Error: function should return %d, actually returned %d", shouldReturn, result);
        return -1;
    }
    else if (shouldReturn != 0) {
        return 1;
    }
    if(universe->last_mass == mass) {
        return 1;
    }
    printf("Mass was not added to end of list. List:\n");
    Mass* buffer = universe->first_mass;
    while (buffer != NULL) {
        printf("Mass = %f, ", buffer->mass_kg);
        buffer = buffer->next_mass;
    }

    return -1;
}

int test_addTwoMassesToUniverse(Universe* universe, Mass* mass, int shouldReturn) {
    Mass* first_mass = universe->first_mass;
    int result1 = test_addOneMassToUniverse(universe, mass, shouldReturn);
    if (result1 != 1) {
        return -1;
    }
    if (universe->first_mass == first_mass && universe->last_mass == mass) {
        return 1;
    }
    return -1;
}

int test_createUniverse(Universe* universe) {
    createUniverse(universe);
    if (universe == NULL) {
        printf("universe is NULL\n");
        return -1;
    }
    return 1;
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
    int total_tests = 0;
    //SET DELTA TIME TESTS
    if(test_set_DELTA_TIME_SECONDS(10.0, 0) != 1)   {number_failed_tests++; printf("failed test %d",total_tests);}
    total_tests++;
    if(test_set_DELTA_TIME_SECONDS(-15.0, -1) != 1) {number_failed_tests++;printf("failed test %d",total_tests);}
    total_tests++;
    if(test_set_DELTA_TIME_SECONDS(0.0, -1) != 1)   {number_failed_tests++;printf("failed test %d",total_tests);}
    total_tests++;

    Universe universe;
    Mass first_mass; Mass second_mass; Mass third_mass;
    if (test_createUniverse(&universe) != 1) {number_failed_tests++; goto doesnt_requires_working_universe;printf("failed test %d",total_tests);}
    total_tests++;
   // printf("%p", &universe);
    //ADDING MASSES TO UNIVERSE TEST
    first_mass = createMass(100,0 ,0, 0,0,0,0,0);
    if (test_addOneMassToUniverse(&universe, &first_mass,0) != 1) {number_failed_tests++;printf("\nfailed test %d",total_tests); goto doesnt_requires_working_universe;}
    total_tests++;
    second_mass =createMass(200,0,0,0,0,0,0,0);
    if (test_addTwoMassesToUniverse(&universe, &second_mass, 0) != 1) {number_failed_tests++;printf("\nfailed test %d",total_tests); goto doesnt_requires_working_universe;}
    total_tests++;
    if (test_addOneMassToUniverse(NULL, &first_mass, -1) !=1 ) {number_failed_tests++;printf("\nfailed test %d",total_tests); goto doesnt_requires_working_universe;};
    total_tests++;
    if (test_addOneMassToUniverse(&universe, NULL, -2) !=1 ) {number_failed_tests++;printf("\nfailed test %d",total_tests); goto doesnt_requires_working_universe;};
    total_tests++;
    doesnt_requires_working_universe:
    printf("\nnumber_failed_tests: %d\n", number_failed_tests);

    //File Read test
    testFileRead();
  //  testCasting();
}
