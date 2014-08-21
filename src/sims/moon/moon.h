#ifndef FUZZYGEN_SIMS_MOON_H
#define FUZZYGEN_SIMS_MOON_H

void MoonCreateVars();
void MoonInitSim(int controller);
int MoonNextStep(int controller);
void DrawMoonSim();
void MoonControlController(int controller);
extern float moon_fuel;
extern float moon_lander_x_pos;
extern float moon_lander_mass;
extern float moon_safe_x_pos;
#endif