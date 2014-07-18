#ifndef MOON_H
#define MOON_H

void MoonCreateVars();
void MoonInitSim(int controller);
int MoonNextStep(int controller);
void DrawMoonSim();
#endif