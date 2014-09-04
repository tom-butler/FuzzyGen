#ifndef FUZZYGEN_SIMS_PENDULUM_H
#define FUZZYGEN_SIMS_PENDULUM_H

#include "..\..\shared\shared.h"

void PendulumCreateVars();
void PendulumInitSim(int controller);
int PendulumNextStep(int controller);
void PendulumControlController(int controller, FuzzyVar input[], Accumulator output[]);
extern float pendulum_cart_position;
extern float pendulum_angular_position;
extern float kRodLength;
extern short int kPendulumSimWidth;
#endif