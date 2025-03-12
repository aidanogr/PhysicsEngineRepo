//
// Created by Aidan O'Grady on 3/12/25.
//

#ifndef UNIVERSEHANDLER_H
#define UNIVERSEHANDLER_H



typedef struct {
    double vx;
    double vy;
    double vz;
    double magnitude;
    unsigned magIsSquared: 1;
} Vector;

struct Mass;
typedef struct Mass Mass;
struct Mass{
    double mass_kg;
    double charge;

    Vector position_meters;
    Vector velocity_meters;

    int index;
    Mass* next_mass;    //DO NOT MANUALLY MODIFY THIS FIELD, handled by addMassToUniverse()
};

typedef struct {
    int number_of_masses;
    Mass* first_mass;            //Individual mass contains pointer to next mass, this is a pointer to first mass
    Mass* last_mass;
} Universe;

typedef struct {
    Vector first_vector;
} Vector_List;



double* DELTA_TIME_SECONDS_POINTER;
int set_DELTA_TIME_SECONDS(double delta_time);


//Universe and Mass Handlers
void createUniverse(Universe* universe);
int addMassToUniverse(Universe* universe, Mass* mass);
Mass createMass(double mass_kg, double charge, double initial_position_m_x, double initial_position_m_y, double initial_position_m_z,
    double initial_velocity_m_per_s_x, double initial_velocity_m_per_s_y, double initial_velocity_m_per_s_z);


//Vector Operations
double distance_between_masses_squared(Mass* first_mass, Mass* second_mass, Vector* vector_between_masses);
int normalizeVector(Vector* vector);


//Force Operations
int applyGravitationalForce(Universe* universe);
int applyElectrostaticForce(Universe* universe);
int applyVelocity(Universe* universe);


#endif //UNIVERSEHANDLER_H
