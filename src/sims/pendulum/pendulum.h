#ifndef FUZZYGEN_SIMS_PENDULUM_H
#define FUZZYGEN_SIMS_PENDULUM_H

#include "..\..\shared\shared.h"

void PendulumCreateVars();
void PendulumInitSim(int controller);
int PendulumNextStep(int controller);
void PendulumControlController(int controller, FuzzyVar input[], Accumulator output[]);
extern const float kCartMoveRadius;
extern const float kPoleLength;
extern const float kMassMass;
extern const int kMaxTime;
extern float score_count;
#endif