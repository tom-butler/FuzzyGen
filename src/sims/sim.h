#ifndef FUZZYGEN_SIMS_SIM_H
#define FUZZYGEN_SIMS_SIM_H

#include "..\shared\shared.h"

void SimCreateSim();
void SimInitSim(int controller);
int SimNextStep(int controller);
void SimControlController(int controller, FuzzyVar input[], Accumulator output[]);

#endif