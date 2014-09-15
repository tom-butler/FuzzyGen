#include <iostream>
#include <stdlib.h>

#include "..\shared\shared.h"
#include "..\shared\gen.h"

using namespace std;

void GALoop();
int main(int argc, char *argv[])
{

  if(argc == 1){  //simple run
    cout << "Initalising System             ";
    InitSystem();
    cout << "OK\n";
    cout << "Initalising Controllers        ";
    InitControllers();
    cout << "OK\n";
    GALoop();
  }
  else if(argc >= 4){ //run with predefined test data
    cout << "Initalising System            ";
    InitSystem();
    cout << "OK\n";
    cout << "Initalising Test              ";
    int r = InitTest(atoi(argv[1]), atoi(argv[2]));
    kRunNum = argv[3];
    if(r == 0) {
      cout << "OK\n";
      cout << "Initalising Controllers       ";
      InitControllers();
      cout << "OK\n";
      GALoop();
    }
    else
      cout << "Failed, using default parameters\n";
  }
  else{

  }

}

void GALoop() {
  for (int g = 0; g < kNumGenerations; g++) {
    BEST_GEN_SCORE = 0;
    cout << "Running Sim                    ";

    for(int c = 0; c < kNumPop; ++c) {
      //if we have random tests run several times
      int score = 0;
      for(int t = 0; t < kNumTests; t++) {
        InitSimulation(c);
        int result = -1;
        while(result == -1) {
          result = RunSim(c);
        }
        score += cont[c].score;
      }
        cont[c].score = score / kNumTests;

        if(cont[c].score > BEST_SCORE) {
          BEST_SCORE = cont[c].score;
          CleanController(BEST_CONTROLLER);
          CopyController(cont[c],BEST_CONTROLLER);
        }

        if(cont[c].score > BEST_GEN_SCORE) {
          BEST_GEN_SCORE = cont[c].score;
          BEST_GEN_CONTROLLER = c;
        }
    }

    cout << "OK\n";
    if(g < kNumGenerations - 1){
      cout << "Breeding Controllers           ";
      Breed();
      cout << "OK\n";
    }
    cout << "GEN " << g << " MAX BEST " << BEST_SCORE << " BEST " << BEST_GEN_SCORE << " AVG " << AVG_GEN << " LOW " << LOW_GEN << "\n";
    //log data
    if(kLogging){
      UpdateLog(g);
    }
  }
  //save the log to file
  if(kLogging){
    cout << "SAVING LOGS                    ";
    WriteLog();
    cout  << "OK\n";
  }
}
