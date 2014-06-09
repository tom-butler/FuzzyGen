#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

//functions
void checkInput(int argc, char *argv[]);
void initSim();

//variables
char *fuzzyData;

int main(int argc, char *argv[]) {
  checkInput(argc,argv);
  initSim();
}

//Extracts data from the input options
//
void checkInput(int argc, char *argv[]) {

  if(argc % 2 != 1) {
    cout << "Error, poorly defined options";
    exit(1);
  }

  for(int i = 1; i < argc; i = i + 2) {
    //fuzzy sets
    if(strcmp(argv[i],"--f")) {
      if(i + 1 < argc) {
        fuzzyData = argv[i+1];
      }
    }
    else {
      cout << "Error, option " << argv[i] << " was not understood";
      exit(1);
    }
  }
}

void initSim() {
  pair<int, int> pos(1.0f,2.0f);
}