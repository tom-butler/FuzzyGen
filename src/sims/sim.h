#ifndef SIM_H
#define SIM_H

void SimCreateSim();
void SimInitSim(int controller);
int SimNextStep(int controller);
void SimControlController(int controller);

#endif