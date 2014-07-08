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
    for(int c = 0; c < POP; c++) {

      InitSimulation(c);
      cout << 2;
      int result = -1;
      while(result == -1) {
        result = RunSim(c);
        cout << 3;
      }
      //cout << "\n\n";
      if(result > BEST){
        BEST = result;
        BEST_CONT = c;
      }
    }
    cout << 4;
    Breed();
    cout << "GEN " << g;
    cout << " BEST " << BEST;
    cout << "\n";
  }
}