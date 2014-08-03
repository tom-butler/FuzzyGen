#ifndef MOON_H
#define MOON_H

void HarrierCreateVars();
void HarrierInitSim(int controller);
int HarrierNextStep(int controller);
void HarrierControlController(int controller);
extern float fuel;
extern float landerX;
extern float landerMass;
extern float safeX;
#endif