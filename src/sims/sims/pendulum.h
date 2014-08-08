#ifndef PENDULUM_H
#define PENDULUM_H

void PendulumCreateVars();
void PendulumInitSim(int controller);
int PendulumNextStep(int controller);
void PendulumControlController(int controller);
extern float pendulumAngle;
extern float centre;
#endif