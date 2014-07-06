#include "..\objects\gen.h"

void GALoop();
int main(int argc, char *argv[])
{
  InitSystem(argc, argv);
  InitControllers();
  GALoop();
}

void GALoop() {
  for (int i = 0; i < GENERATIONS; i++) {
    ScoreFitnesses();
    Breed();
    cout << "GEN " << i;
    cout << " BEST " << BEST;
    cout << "\n";
  }
}