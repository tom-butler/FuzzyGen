#include "..\objects\gen.h"

void GALoop();
int main(int argc, char *argv[])
{
  InitSystem(argc, argv);
  InitControllers();
  GALoop();
}

void GALoop() {
  for (int g = 0; g < GENERATIONS; g++) {
    for(int c = 0; c < POP; c++) {
      initSim(i);
      int result = -1;
      while(result == -1) {
        result = RunSim();
      }
      //cout << "\n\n";
      if(result > BEST){
        BEST = result;
        BEST_CONT = i;
      }
    }
    Breed();
    cout << "GEN " << g;
    cout << " BEST " << BEST;
    cout << "\n";
  }
}