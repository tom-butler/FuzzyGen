#ifndef HARRIER_H
#define HARRIER_H

extern float harrier_YPos;
extern float harrier_XPos;
extern float harrier_fuel;
//extern float RelativeXVel;
extern float harrier_mass;

extern float harrier_safeX;
extern float harrier_safeY;
extern float harrier_safeWidth;

extern int HARRIER_SIM_WIDTH;
extern int HARRIER_SIM_HEIGHT;

void HarrierCreateVars();
void HarrierInitSim(int controller);
int HarrierNextStep(int controller);
void HarrierControlController(int controller);

#endif