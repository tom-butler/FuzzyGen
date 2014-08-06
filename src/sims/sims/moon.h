#ifndef MOON_H
#define MOON_H

void MoonCreateVars();
void MoonInitSim(int controller);
int MoonNextStep(int controller);
void DrawMoonSim();
void MoonControlController(int controller);
extern float moon_fuel;
extern float moon_landerX;
extern float moon_landerMass;
extern float moon_safeX;
#endif