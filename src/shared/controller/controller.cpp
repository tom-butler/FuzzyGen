#include "controller.h"

#include "..\shared.h"
#include "h\create.h"
#include "h\run.h"
#include "h\select.h"

void Create(int num, FuzzyVar input[], Accumulator output[]){
  CreateControllers(num,input,output);
}
void Run(int controller){
  RunController(controller);
}
void Select(){
  SelectControllers();
}