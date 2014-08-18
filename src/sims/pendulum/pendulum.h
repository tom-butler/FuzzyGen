#ifndef FUZZYGEN_SIMS_PENDULUM_H
#define FUZZYGEN_SIMS_PENDULUM_H

void PendulumCreateVars();
void PendulumInitSim(int controller);
int PendulumNextStep(int controller);
void PendulumControlController(int controller);
extern float pendulum_cartPosition;
extern float pendulum_angularPosition;
extern float ROD_LENGTH;
extern short int PENDULUM_SIM_WIDTH;
extern short int pendulum_score;
#endif