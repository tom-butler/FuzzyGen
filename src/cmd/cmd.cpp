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
  else if(argc >= 2){ //run with predefined test data
    cout << "Initalising Test              ";
    int r = InitTest(atoi(argv[1]));
    if(r == 0)
      cout << "OK\n";
    else
      cout << "Failed, using default parameters\n";
  }
  if(argc >= 3){ //looped test run
    for(int run = 0; run < atoi(argv[2]); run++){
      cout << "Initalising System            ";
      InitSystem();
      cout << "OK\n";
      cout << "Initalising Test              ";
      int r = InitTest(atoi(argv[1]));
      if(r == 0)
        cout << "OK\n";
      else
        cout << "Failed, using default parameters\n";

      cout << "Initalising Controllers       ";
      InitControllers();
      cout << "OK\n";
      GALoop();
    }
  }
  else{

  }

}

void GALoop() {
  for (int g = 0; g < kNumGenerations; g++) {
    BEST_GEN_SCORE = 0;
    cout << "Running Sim                    ";
    for(int c = 0; c < kNumPop; ++c) {
      InitSimulation(c);
      int result = -1;
      while(result == -1) {
        result = RunSim(c);
      }
      if(cont[c].score > BEST_SCORE){
        BEST_SCORE = cont[c].score;
      }
      if(cont[c].score > BEST_GEN_SCORE){
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
    cout << "GEN " << g << " MAX BEST " << BEST_SCORE << " BEST " << BEST_GEN_SCORE << " MEAN " << MEAN_GEN << " LOW " << LOW_GEN << "\n";
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
