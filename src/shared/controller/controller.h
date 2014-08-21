#ifndef FUZZYGEN_SHARED_CONTROLLER_CONTROLLER_H
#define FUZZYGEN_SHARED_CONTROLLER_CONTROLLER_H

//Forward Declarations
#include "..\shared.h"

void Create(int num, FuzzyVar input[], Accumulator output[]);
void Run(int controller);
void Select();

#endif