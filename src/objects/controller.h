#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include "shared.h"

void CreateControllers(int num, FuzzyVar input[], Accumulator output);
void EvaluateRules(int controller);
void BreedControllers();

#endif