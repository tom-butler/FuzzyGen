
#include "..\..\shared.h"

#include "..\h\run.h"

float EvaluateRule(int controller, int accumulator, int rule); 
float EvaluateSet(int controller, int inputVar, int setID, int variable);
void EvaluateOutput(int controller, int accumulator);

//evaluate all rules that have a single output
void RunController(int controller) {
  for(int o = 0; o < NUM_OUTPUT; o++){
    ResetAccumulator(controller, o);

    for(int i = 0; i < cont[controller].output[o].ruleNum; i++) {
      cont[controller].output[o].rules[i].isActive = false;
      float variable = EvaluateRule(controller,o, i);
      
      //add result
      if(variable > 0.0f) {
        cont[controller].output[o].rules[i].isActive = true;
        cont[controller].output[o].scale[cont[controller].output[o].active] = variable;
        cont[controller].output[o].value[cont[controller].output[o].active] = cont[controller].output[o].rules[i].output;
        cont[controller].output[o].active++;
      }
    }
    EvaluateOutput(controller, o);
  }
}

float EvaluateRule(int controller, int accumulator, int rule){
  cont[controller].output[accumulator].rules[rule].isActive = false;
  float bestResult = cont[controller].output[accumulator].high;
  for(int i = 0; i < cont[controller].output[accumulator].varsNum; i++){
    float result = EvaluateSet(controller, cont[controller].output[accumulator].vars[i], cont[controller].output[accumulator].rules[rule].sets[i], cont[controller].input[cont[controller].output[accumulator].vars[i]].value);
    if(result < bestResult)
      bestResult = result;
  }
  return bestResult;
}

//@TODO: repalce sets[setID] with a pointer
float EvaluateSet(int controller, int inputVar, int setID, int variable) {
  Set set = cont[controller].input[inputVar].sets[setID];
  //check if it is inside the set
  if(variable >= (set.centreX - set.leftBase) && variable <= ( set.centreX + set.rightBase))
    if(variable < set.centreX) //left
      if(variable < (set.centreX - set.leftTop))
        return Intersect(set.centreX - set.leftBase, 0.0f, set.centreX - set.leftTop, set.height, variable);
      else //variable > leftTop - centreX
        return set.height;
    else //right or centre
      if(variable > (set.centreX + set.rightTop))
        return Intersect(set.centreX + set.rightTop, set.height, set.centreX + set.rightBase, 0.0f,variable);
      else
        return set.height;
  else
    return 0;
}

void EvaluateOutput(int controller, int accumulator) {
  if(cont[controller].output[accumulator].active == 0)
    return;

  float total = 0.0f;
  float totalWeight = 0.0f;
  for(int i = 0; i < cont[controller].output[accumulator].active; i++) {
   total += cont[controller].output[accumulator].scale[i] * cont[controller].output[accumulator].value[i];
   totalWeight += cont[controller].output[accumulator].scale[i];
  }

  total /= totalWeight;
  cont[controller].output[accumulator].output = total;
}