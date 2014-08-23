#ifndef FUZZYGEN_SIMS_PENDULUM_H
#define FUZZYGEN_SIMS_PENDULUM_H

void PendulumCreateVars();
void PendulumInitSim(int controller);
int PendulumNextStep(int controller);
void PendulumControlController(int controller);
extern float pendulum_cart_position;
extern float pendulum_angular_position;
extern float kRodLength;
extern short int kPendulumSimWidth;
extern float pendulum_score;
#endif