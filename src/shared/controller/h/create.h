#ifndef FUZZYGEN_SHARED_CONTROLLER_CREATE_H
#define FUZZYGEN_SHARED_CONTROLLER_CREATE_H

//Forward Declarations
#include "..\..\shared.h"

void CreateControllers(int num, FuzzyVar input[], Accumulator output[]);
void CreateSets(int controller, int variable, short int numSets);
void CreateRules(int controller, int accumulator);

#endif