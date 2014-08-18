#include <iostream>
#include <stdlib.h>

#include "..\objects\shared.h"
#include "..\objects\gen.h"

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
  for (int g = 0; g < GENERATIONS; g++) {
    BEST = 0;
    cout << "Running Sim                    ";
    for(int c = 0; c < POP; ++c) {
      InitSimulation(c);
      int result = -1;
      while(result == -1) {
        result = RunSim(c);
      }
      if(cont[c].score > MAX_BEST){
        MAX_BEST = cont[c].score;
      }
      if(cont[c].score > BEST){
        BEST = cont[c].score;
        BEST_CONT = c;
      }
      
    }
    cout << "OK\n";
    if(g < GENERATIONS-1){
      cout << "Breeding Controllers           ";
      Breed();
      cout << "OK\n";
    }
    cout << "GEN " << g << " MAX BEST " << MAX_BEST << " BEST " << BEST << " MEAN " << MEAN << " LOW " << LOW << "\n";
    //log data
    if(LOGGING){
      UpdateLog(g);
    }
  }
  //save the log to file
  if(LOGGING){
    cout << "SAVING LOGS                    ";
    WriteLog();
    cout  << "OK\n";
  }
}
