#ifndef GEN_H
#define GEN_H

//shared

//functions
void InitSystem(int argc, char *argv[]);
void InitControllers();
void InitSimulation(int controller);
int RunSim(int controller);
void Breed();
void SaveBest();

#endif