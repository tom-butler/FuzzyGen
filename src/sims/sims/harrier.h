#ifndef MOON_H
#define MOON_H

void HarrierCreateVars();
void HarrierInitSim(int controller);
int HarrierNextStep(int controller);
void HarrierControlController(int controller);
extern float fuel;
extern float RelativeXPos;
extern float landerMass;
extern float safeX;
extern float safeY;
extern float safeWidth;
extern int SIM_WIDTH;
extern int SIM_HEIGHT;
#endif