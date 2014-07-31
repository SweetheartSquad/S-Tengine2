#pragma once;

#define SIM_SIZE 64 //height width and depth of simulation
#define NUM_DIMENSIONS 2 //boids' degree of freedom. max of 3
#define NEIGHBOUR_DISTANCE 8 //radius of influence for neighbourhood (should be smaller than SIM_SIZE)
#define COLLISION_RADIUS 1 //radius of influence for collision avoidance
#define NUM_BOIDS 250 //number of boids
#define PREDATOR_RATIO 0.0 //likelihood of boid being predator (0 to 1)
#define MAX_NEIGHBOURS NUM_BOIDS //maximum neighbours per neighbourhood

#define DEFAULT_SPEED 0.1
#define CHANGE_LIMIT 0.1 //Maximum change in orientation per frame as percentage (1 = 180)
#define COEFFICIENT_OF_FRICTION 0.9
#define FILE_TYPE "PRT"

#define PI 3.141592653589793

#define GRID_IDX(_x, _y, _z) ((long int)((double)( ((_x)/(NEIGHBOUR_DISTANCE))*((SIM_SIZE)/(NEIGHBOUR_DISTANCE))*((SIM_SIZE)/(NEIGHBOUR_DISTANCE))+((_y)/(NEIGHBOUR_DISTANCE))*((SIM_SIZE)/(NEIGHBOUR_DISTANCE))+(_z)/(NEIGHBOUR_DISTANCE) )))
#define NUM_CELLS GRID_IDX((SIM_SIZE), (SIM_SIZE), (SIM_SIZE))

//#define PROFILING

//#define PAUSE_EACH_STEP

#define ACCELERATED