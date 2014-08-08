#ifndef CONT_CREATE_H
#define CONT_CREATE_H

#include "..\..\shared.h"

void CreateControllers(int num, FuzzyVar input[], Accumulator output[]);
void CreateRules(int controller, int accumulator);

#endif