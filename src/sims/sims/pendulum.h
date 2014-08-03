#ifndef MOON_H
#define MOON_H

void PendulumCreateVars();
void PendulumInitSim(int controller);
int PendulumNextStep(int controller);
void PendulumControlController(int controller);
extern float pendulumAngle;
extern float centre;
#endif