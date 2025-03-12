//
// Created by Aidan O'Grady on 3/12/25.
//

#include "UniverseHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "UNIVERSAL_CONSTANTS.h"
#include "UniverseHandler.h"
#include "Animate.h"
//MASS HANDLERS

//default mass generator, no energy init
Mass createMass(double mass_kg, double charge, double initial_position_m_x, double initial_position_m_y, double initial_position_m_z,
    double initial_velocity_m_per_s_x, double initial_velocity_m_per_s_y, double initial_velocity_m_per_s_z) {
    printf("DEltaTimeSeconds2: %f\n", DELTA_TIME_SECONDS);

    Vector initial_position_meters = { initial_position_m_x, initial_position_m_y, initial_position_m_z };
    Vector initial_velocity_meters = { initial_velocity_m_per_s_x, initial_velocity_m_per_s_y, initial_velocity_m_per_s_z };

    Mass m = {mass_kg, charge,
        initial_position_meters,
        initial_velocity_meters, 0};
    return m;
}




//UNIVERSE HANDLERS, USE THESE INSTEAD OF MANUALLY MODIFYING UNIVERSE AND MASS STRUCTS

/**
* Appends mass to universe struct
*
*  Returns  0 if successful
*  Returns -1 if universe is null pointer
*  Returns -2 if mass is null pointer
*  Returns -3 if universe is not properly initialized/configured
*/
int addMassToUniverse(Universe* universe, Mass* mass) {
  if(universe == NULL) {
    return -1;
  }
  else if(mass == NULL) {
      return -2;
  }

  //default initialization of new created universe...
  if(universe->first_mass == NULL && universe->last_mass == NULL && universe->number_of_masses == 0) {
      mass->next_mass = NULL;
      mass->index = 0;

      universe->first_mass = mass;
      universe->last_mass = mass;
      universe->number_of_masses = 1;
  }
  //bad configuration or initialization
  else if(universe->first_mass == NULL || universe->last_mass == NULL) {
      return -3;
  }
  //now that we have 2 masses, we can properly create a Singly Linked List
  else if (universe->number_of_masses == 1) {
      universe->first_mass->next_mass = mass;
      universe->last_mass = mass;
      mass->next_mass = NULL;
      mass->index = 1;
      universe->number_of_masses = 2;
  }
  else {
    universe->last_mass->next_mass = mass;
    mass->index = universe->last_mass->index + 1;
    universe->last_mass = mass;
    universe->number_of_masses++;
  }
  return 0;
}


void createUniverse(Universe* universe) {
    universe->number_of_masses = 0;
    universe->first_mass = NULL;
    universe->last_mass = NULL;
}



//FORCE APPLICATIONS

//If desired, a Vector can be passed. The function will fill it with position vector m1->m2
//Note that vector->magnitude will be stored as magnitude^2 and vector->magIsSquared = 1;
//Otherwise, pass NULL and function behaves as expected
double distance_between_masses_squared(Mass* first_mass, Mass* second_mass, Vector* vector_between_masses) {
    if(vector_between_masses == NULL) {
        return ((first_mass->position_meters.vx - second_mass->position_meters.vx) *
            (first_mass->position_meters.vx - second_mass->position_meters.vx) +
            (first_mass->position_meters.vy - second_mass->position_meters.vy) *
            (first_mass->position_meters.vy - second_mass->position_meters.vy) +
            (first_mass->position_meters.vz - second_mass->position_meters.vz) *
            (first_mass->position_meters.vz - second_mass->position_meters.vz));
    }
    else {
        vector_between_masses->vx = first_mass->position_meters.vx - second_mass->position_meters.vx;
        vector_between_masses->vy = first_mass->position_meters.vy - second_mass->position_meters.vy;
        vector_between_masses->vz = first_mass->position_meters.vz - second_mass->position_meters.vz;
        vector_between_masses->magnitude =
            vector_between_masses->vx * vector_between_masses->vx +
            vector_between_masses->vy * vector_between_masses->vy +
            vector_between_masses->vz * vector_between_masses->vz;
        vector_between_masses->magIsSquared = 1;
        return (vector_between_masses->magnitude);
    }
}


int normalizeVector(Vector* vector) {
  if(vector->magIsSquared) {
    vector->magnitude = sqrt(vector->magnitude);
   }
  if (vector->magnitude != 0) {
      vector->vx /= vector->magnitude;
      vector->vy /= vector->magnitude;
      vector->vz /= vector->magnitude;
      return 0;
  }
  else {
      return -1;
  }
}

int applyGravitationalForce(Universe* universe) {
    if(universe->number_of_masses == 0 || universe->number_of_masses == 1) { return -2; }
    Mass* anchor_mass = universe->first_mass;
    Mass* buffer_mass  = universe->first_mass;

    int buffer_index = anchor_mass->index;
    int anchor_index = anchor_mass->index;

    double distance_squared = 0;
    double acceleration_anchor_magnitude = 0;
    Vector position_vector = {0,0,0};

    while(anchor_index < universe->number_of_masses) {
        buffer_index = anchor_index;
        buffer_mass = anchor_mass;
        while (buffer_index < universe->number_of_masses-1) {
            if (buffer_mass == NULL || buffer_mass->next_mass == NULL) {
                return -1;
            }

            buffer_mass = buffer_mass->next_mass;
            buffer_index++;

            //function also stores distance as a vector
            distance_squared = distance_between_masses_squared(anchor_mass, buffer_mass, &position_vector);
            acceleration_anchor_magnitude =
                (GRAVITATIONAL_CONSTANT * buffer_mass->mass_kg)/(-1.0*distance_squared);

            normalizeVector(&position_vector);
            anchor_mass->velocity_meters.vx += acceleration_anchor_magnitude * position_vector.vx * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vy += acceleration_anchor_magnitude * position_vector.vy * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vz += acceleration_anchor_magnitude * position_vector.vz * DELTA_TIME_SECONDS;

        }
        buffer_index = 0;
        buffer_mass = universe->first_mass;
        while(buffer_index < anchor_index) {
            if (buffer_mass == NULL || buffer_mass->next_mass == NULL ) {
                return -1;
            }

            //function also stores distance as a vector
            distance_squared = distance_between_masses_squared(anchor_mass, buffer_mass, &position_vector);
            acceleration_anchor_magnitude =
                (GRAVITATIONAL_CONSTANT  * buffer_mass->mass_kg)/(-1.0*distance_squared);

            if (normalizeVector(&position_vector) != 0) return -4;
            anchor_mass->velocity_meters.vx += acceleration_anchor_magnitude * position_vector.vx * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vy += acceleration_anchor_magnitude * position_vector.vy * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vz += acceleration_anchor_magnitude * position_vector.vz * DELTA_TIME_SECONDS;

            buffer_mass = buffer_mass->next_mass;
            buffer_index++;
        }
        anchor_index++;
        if (anchor_mass->next_mass != NULL)
        anchor_mass = anchor_mass->next_mass;
    }
    return 0;
}

int applyElectrostaticForce(Universe* universe) {

    if (universe == NULL) { return -1; }
    if(universe->number_of_masses == 0) { return -2; }
    if(universe->number_of_masses == 1) { return -3; }

    Mass* anchor_mass = universe->first_mass;
    Mass* buffer_mass  = universe->first_mass;

    int buffer_index = anchor_mass->index;
    int anchor_index = anchor_mass->index;

    double distance_squared = 0;
    double acceleration_anchor_magnitude = 0;
    Vector position_vector = {0,0,0};
    Vector force_m1_m2_vector_normalized = {0,0,0};

    while(anchor_index < universe->number_of_masses) {
        buffer_index = anchor_index;
        buffer_mass = anchor_mass;
        while (buffer_index < universe->number_of_masses-1) {
            if (buffer_mass == NULL || buffer_mass->next_mass == NULL) {
                return -1;
            }

            buffer_mass = buffer_mass->next_mass;
            buffer_index++;

            //function also stores distance as a vector
            distance_squared = distance_between_masses_squared(anchor_mass, buffer_mass, &position_vector);
            acceleration_anchor_magnitude =
                (COULOMBS_CONSTANT * buffer_mass->charge * anchor_mass->charge)/(1.0*distance_squared*anchor_mass->mass_kg);

            normalizeVector(&position_vector);
            anchor_mass->velocity_meters.vx += acceleration_anchor_magnitude * position_vector.vx * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vy += acceleration_anchor_magnitude * position_vector.vy * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vz += acceleration_anchor_magnitude * position_vector.vz * DELTA_TIME_SECONDS;

        }
        buffer_index = 0;
        buffer_mass = universe->first_mass;
        while(buffer_index < anchor_index) {
            if (buffer_mass == NULL || buffer_mass->next_mass == NULL ) {
                return -1;
            }

            //function also stores distance as a vector
            distance_squared = distance_between_masses_squared(anchor_mass, buffer_mass, &position_vector);
            acceleration_anchor_magnitude =
                (COULOMBS_CONSTANT  * buffer_mass->charge * anchor_mass->charge)/(1.0*distance_squared*anchor_mass->mass_kg);

            if (normalizeVector(&position_vector) != 0) return -4;
            anchor_mass->velocity_meters.vx += acceleration_anchor_magnitude * position_vector.vx * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vy += acceleration_anchor_magnitude * position_vector.vy * DELTA_TIME_SECONDS;
            anchor_mass->velocity_meters.vz += acceleration_anchor_magnitude * position_vector.vz * DELTA_TIME_SECONDS;

            buffer_mass = buffer_mass->next_mass;
            buffer_index++;
        }
        anchor_index++;
        if (anchor_mass->next_mass != NULL)
        anchor_mass = anchor_mass->next_mass;
    }
    return 0;


}


int applyVelocity(Universe* universe) {
    if(universe->number_of_masses == 0) { return -1; }
    Mass* buffer_mass = universe->first_mass;
    int buffer_index = 0;
    while (buffer_index < universe->number_of_masses) {
        if (buffer_mass == NULL) { return -2;}

        buffer_mass->position_meters.vx += buffer_mass->velocity_meters.vx * DELTA_TIME_SECONDS;
        buffer_mass->position_meters.vy += buffer_mass->velocity_meters.vy * DELTA_TIME_SECONDS;
        buffer_mass->position_meters.vz += buffer_mass->velocity_meters.vz * DELTA_TIME_SECONDS;
        buffer_mass = buffer_mass->next_mass;
        buffer_index ++;
    }
    return 0;

}
