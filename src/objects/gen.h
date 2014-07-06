#ifndef GEN_H
#define GEN_H

#include <string>
#include "controller.h"


//shared
static Controller *cont;

//functions
void InitSystem(int argc, char *argv[]);
void InitControllers();
void InitSim(int controller);
int RunSim(int controller);
void Breed();
#endif