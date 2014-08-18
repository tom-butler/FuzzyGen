#ifndef FUZZYGEN_SHARED_GEN_H
#define FUZZYGEN_SHARED_GEN_H

void InitControllers();
void InitSimulation(int controller);
int RunSim(int controller);
void Breed();
void UpdateLog(int g);
void DrawSim();
void WriteLog();


#endif