#include "..\objects\shared.h"
#include "..\objects\gen.h"
#include <iostream>

using namespace std;

void GALoop();
int main(int argc, char *argv[])
{
  cout << "Initalising System            ";
  InitSystem(argc, argv);
  cout << "OK\n";
  cout << "Initalising Controllers       ";
  InitControllers();
  cout << "OK\n";
  GALoop();
}

void GALoop() {
  for (int g = 0; g < GENERATIONS; g++) {
    cout << "Running Sim                    ";
    for(int c = 0; c < POP; ++c) {
      InitSimulation(c);
      int result = -1;
      while(result == -1) {
        result = RunSim(c);
      }
      if(cont[c].score > BEST){
        BEST = cont[c].score;
        BEST_CONT = c;
      }
    }
    cout << "OK\n";
    cout << "Breeding Controllers           ";
    Breed();
    cout << "OK\n";
    cout << "GEN " << g;
    cout << " BEST " << BEST;
    cout << " MEAN " << MEAN;
    cout << " LOW " << LOW;
    cout << "\n";
  }
}