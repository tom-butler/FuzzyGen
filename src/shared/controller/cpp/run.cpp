#include "..\h\run.h"

#include "..\..\shared.h"


float EvaluateRule(int controller, int accumulator, int rule); 
float EvaluateSet(int controller, int inputVar, int setID, int variable);
void EvaluateOutput(int controller, int accumulator);

//evaluate all rules that have a single output
void RunController(int controller) {
  for(int o = 0; o < kNumOutput; o++){
    ResetAccumulator(controller, o);
    cont[controller].score = 0;
    for(int i = 0; i < cont[controller].output[o].num_rules; i++) {
      cont[controller].output[o].rules[i].is_active = false;
      float variable = EvaluateRule(controller,o, i);
      
      //add result
      if(variable > 0.0f) {
        cont[controller].output[o].rules[i].is_active = true;
        cont[controller].output[o].scale[cont[controller].output[o].num_active] = variable;
        cont[controller].output[o].value[cont[controller].output[o].num_active] = cont[controller].output[o].rules[i].output;
        cont[controller].output[o].num_active++;
      }
    }
    EvaluateOutput(controller, o);
  }
}

float EvaluateRule(int controller, int accumulator, int rule){
  cont[controller].output[accumulator].rules[rule].is_active = false;
  float best_result = cont[controller].output[accumulator].high;
  for(int i = 0; i < cont[controller].output[accumulator].num_vars; i++){
    float result = EvaluateSet(controller, cont[controller].output[accumulator].vars[i], cont[controller].output[accumulator].rules[rule].sets[i], cont[controller].input[cont[controller].output[accumulator].vars[i]].value);
    if(result < best_result)
      best_result = result;
  }
  return best_result;
}

float EvaluateSet(int controller, int inputVar, int setID, int variable) {
  Set set = cont[controller].input[inputVar].sets[setID];
  //check if it is inside the set
  if(variable >= (set.centre_x - set.left_base) && variable <= ( set.centre_x + set.right_base))
    if(variable < set.centre_x) //left
      if(variable < (set.centre_x - set.left_top))
        return Intersect(set.centre_x - set.left_base, 0.0f, set.centre_x - set.left_top, set.height, variable);
      else //variable > left_top - centre_x
        return set.height;
    else //right or centre
      if(variable > (set.centre_x + set.right_top))
        return Intersect(set.centre_x + set.right_top, set.height, set.centre_x + set.right_base, 0.0f,variable);
      else
        return set.height;
  else
    return 0;
}

void EvaluateOutput(int controller, int accumulator) {
  if(cont[controller].output[accumulator].num_active == 0)
    return;

  float total = 0.0f;
  float total_weight = 0.0f;
  for(int i = 0; i < cont[controller].output[accumulator].num_active; i++) {
   total += cont[controller].output[accumulator].scale[i] * cont[controller].output[accumulator].value[i];
   total_weight += cont[controller].output[accumulator].scale[i];
  }

  total /= total_weight;
  cont[controller].output[accumulator].output = total;
}