#ifndef FUZZYGEN_SIMS_HARRIER_H
#define FUZZYGEN_SIMS_HARRIER_H

#include "..\..\shared\shared.h"

void HarrierCreateVars();
void HarrierInitSim(int controller);
int HarrierNextStep(int controller);
void HarrierControlController(int controller, FuzzyVar input[], Accumulator output[]);

extern float harrier_y_position;
extern float harrier_x_position;
extern float harrier_fuel;
//extern float RelativeXVel;
extern float harrier_mass;

extern float harrier_safe_x_position;
extern float harrier_safe_y_position;
extern float harrier_safe_width;

extern int kHarrierSimWidth;
extern int kHarrierSimHeight;

#endif