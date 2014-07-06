#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include "..\settings.h"
//this file defines the structs that are used in the fuzzy logic controller

//functions--------------------------------------------------------------------

void CreateControllers(int num, FuzzyVar input[], Accumulator output);

void EvaluateRules(int controller);

void BreedControllers();


#endif