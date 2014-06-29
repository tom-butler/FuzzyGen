#include "controller.h"

static Controller *cont;
//counters
static int currentController = 0;
static int currentRule = 0;

//util
void ThrowError(string error)
{
  cout << error;
}

int GetRandInt(int low, int high){
  return rand() % (high - low) + low;
}

//find the intersection of two lines
//@TODO: check if parallel lines picks the highest point
float Intersect(int x1, int y1, int x2, int y2, int input) {
  float m, b;
  //find line equation y = mx + b
  m = (y2-y1)/(x2-x1);
  b = y1 -m*x1;
  //get the y value of the intersection
  return m * input + b; ;
}

void UpdateVars(int controller, int newValues[]) {
  for(int i = 0; i < NUM_VARS; i++) {
    cont[controller].vars[i].value = newValues[i];
  }
}

void ScoreController(int controller, int score) {
  cont[controller].score = score;
}

//initialisation
void CreateControllers(int num_controllers, FuzzyVar newVars[]) {
   cont = new Controller[num_controllers];
   while(currentController < num_controllers) {
    //cont[currentController] = {0,0};
    cont[currentController].vars = newVars;
    for(int i = 0; i < NUM_VARS; i++)
    {
      cont[currentController].vars[i].sets = new Set[NUM_SETS];
      InitSets(i, NUM_SETS);

      //if this is an output make some rules
      if(cont[currentController].vars[i].output = i) {
        cont[currentController].rules = new Rule[NUM_SETS];
        InitRules(i);
      }
    }
    currentController++;
  }
}

//@TODO: REPLACE MULTIPLE ARRAY CALLS WITH POINTER
void InitSets(int variable, int numSets) {
  //create initial variables
  int start = cont[currentController].vars[variable].low;
  int end = cont[currentController].vars[variable].high;
  int space = (end - start)  / (numSets - 1);
  int centre = start;
  int bWidth = 0.7 * space;
  int tWidth = 0.3 * space;

  for(int j = 0; j < numSets; j++) {
    //attempt to create set variables
    int lbase = bWidth - centre;
    int rbase = bWidth + centre;
    int ltop = tWidth - centre;
    int rtop = tWidth + centre;

    //check set variables for compliance
    if(lbase  < start)
      lbase = Intersect(0, lbase, 0, centre, start);
    if(rbase > end)
      rbase = Intersect(0, centre, 0, rbase, end);
    if(ltop < start)
      lbase = Intersect(HEIGHT, ltop, HEIGHT, centre, start);
    if(rtop > end)
      rtop = Intersect(HEIGHT, centre, HEIGHT, rtop, end);

    //build the set
    Set s = {centre, HEIGHT, rbase, ltop, rtop, variable};
    cont[currentController].vars[variable].sets[j] = s;
    //increment the centre for next set
    centre += space;
  }
}

//initialises all rules for a given output
void InitRules(int output) {
  for(int i = 0; i < NUM_VARS; i++) {
    if(cont[currentController].vars[i].output != i) {
      for(int j = 0; j < NUM_SETS; j++) {

        for(int k = 0; k < NUM_VARS; k++) {
          if(cont[currentController].vars[k].output != k && i != k) {
            for(int l = 0; l < NUM_SETS; l++) {
              random = GetRandInt(0, NUM_SETS);
              Rule r = {i, j, "AND", k, l, output, random};
              cont[currentController].rules[currentRule] = r;
              currentRule++;
            }
          }
        }
      }
    }
  }
}
//@TODO: THIS NEEDS A RE-WRITE (DAFUQ WAS I DOING?)
//evaluate all rules that have a single output
float EvaluateRules(int controller, int outputID) {
  int variable, res1, res2, result, rcount = 0;
  for(int i = 0; i < NUM_RULES; i++) {
    if(cont[controller].rules[i].outputvar = outputID) {
      rcount++;
      res1 = EvaluateSet(controller, cont[controller].rules[i].inputvar, cont[controller].rules[i].inputset);
      res2 = EvaluateSet(controller, cont[controller].rules[i].inputvar2, cont[controller].rules[i].inputset2);

      //decide on the value to pass to output

      if(cont[controller].rules[i].modifier.compare("AND") == 0) {
          if(res1 < res2)
            variable = res1;
          else
            variable = res2;
      }
      else if(cont[controller].rules[i].modifier.compare("OR") == 0) {
          if(res1 > res2)
            variable = res1;
          else
            variable = res2;
      }
      cont[controller].vars[cont[controller].rules[i].outputvar].value = variable;
      //add result
      result += EvaluateSet(controller,  cont[controller].rules[i].outputvar, cont[controller].rules[i].outputset);

    }
  }
  return result / rcount;
}

//@TODO: repalce sets[setID] with a pointer
float EvaluateSet(int controller, int inputVar, int setID) {
  float variable = cont[controller].vars[inputVar].value;
  Set set = cont[controller].vars[inputVar].sets[setID];
  if(variable > set.leftBase - set.centreX && variable < set.rightBase + set.centreX)
    if(variable < set.centreX)
      if(variable < set.leftTop - set.centreX)
        return Intersect(set.leftBase, 0, set.leftTop, set.height, variable);
      else //variable > leftTop - centreX
        return set.height;
    else //variable > centreX
      if(variable > set.rightTop + set.centreX)
        return Intersect(set.rightBase, 0, set.rightTop, set.height, variable);
      else
        return set.height;
  else
    return 0;
}

void SelectController() {
  //select highest half and breed them
  Controller parents[ANCESTOR];
  int count, avg = 0;

  //find the average score
  for(int i = 0; i < POP; i++)
    avg += cont[i].score;

  avg /= POP;

  //get all controllers with >= avg score
  for(int i = 0; i < POP; i++) {
    if(cont[i].score >= avg && count < ANCESTOR) {
      parents[count] = cont[i];
      count++;
    }
  }

  //if the average didn't give enough parents, add some more randomly
  if(count < ANCESTOR){
    for(int i = count; i < ANCESTOR; i++){
      parents[i] = cont[GetRandInt(0,POP)];
    }
  }


  //breed the parents
  currentController = 0;
  for(int i = 0; i < POP/2; i += 2) {
    BreedController(parents[i], parents[i+1]);
  }
}
//breeding
void BreedController(Controller cont1, Controller cont2) {

  //save the parents
  cont[currentController] = cont1;
  currentController++;
  cont[currentController] = cont2;
  currentController++;

  //create two children from two parents
  cont[currentController] = cont1;
  currentController++;
  cont[currentController] = cont2;
  currentController++;

  //get the id's of the children
  int id1 = currentController -1;
  int id2 = currentController;


  //parent mutation, twice for extra variation
  ParentMutation(id1, id2);
  ParentMutation(id1, id2);

  //self mutation
  ChildMutation(id1);
  ChildMutation(id2);
}


//mutation
void ParentMutation(int id1, int id2) {
  //randomly merge two genes

  //find a random var
  random = GetRandInt(0, 4);
  int col = GetRandInt(0, NUM_VARS -1);

  switch(random) {
    case 0: //swap collections
    {
      FuzzyVar temp = cont[id2].vars[col];
      cont[id2].vars[col] = cont[id1].vars[col];
      cont[id1].vars[col] = temp;
      break;
    }
    case 1: //copy collection
      cont[id2].vars[col] = cont[id1].vars[col];
      break;
    case 2: //swap set
    {
      int set = GetRandInt(0, NUM_SETS -1);
      Set temp = cont[id2].vars[col].sets[set];
      cont[id2].vars[col].sets[set] = cont[id1].vars[col].sets[set];
      cont[id1].vars[col].sets[set] = temp;
      break;
    }
    case 3: //copy set
    {
      int set = GetRandInt(0, NUM_SETS -1);
      cont[id2].vars[col].sets[set] = cont[id1].vars[col].sets[set];
      break;
    }
    case 4: //change rule output
    {
      int rule = GetRandInt(0,NUM_RULES -1);
      Rule temp = cont[id2].rules[rule];
      int rule2 = GetRandInt(0,NUM_RULES -1);
      cont[id2].rules[rule].outputset = cont[id1].rules[rule2].outputset;
      break;
    }
  }
}

void ChildMutation(int id) {
  random = GetRandInt(0, 1);
  if(random == 0)
    MutateSet(id, GetRandInt(0,NUM_VARS), GetRandInt(0,NUM_SETS));
  else
    MutateRule(id, GetRandInt(0, NUM_RULES));
}

//@TODO:POINTER THAT SHIZ
void MutateSet(int controller, int var, int setID) {
  int mut = GetRandInt(0,3);
  random = GetRandInt(-VARIANCE, VARIANCE);
  cont[controller].mutations++;
  switch(mut){
    case 0: //grow top
      cont[controller].vars[var].sets[setID].leftTop -= random;
      cont[controller].vars[var].sets[setID].rightTop += random;
      break;
    case 1: //grow bottom
      cont[controller].vars[var].sets[setID].leftBase -=random;
      cont[controller].vars[var].sets[setID].rightBase +=random;
      break;
    case 2: //slide top
      cont[controller].vars[var].sets[setID].leftTop +=random;
      cont[controller].vars[var].sets[setID].rightTop +=random;
      break;
    case 3: //slide bottom
      cont[controller].vars[var].sets[setID].leftBase +=random;
      cont[controller].vars[var].sets[setID].rightBase +=random;
      break;
    default:
      return;
      break;
  }
  //ensure they are in bounds
  if(cont[controller].vars[var].sets[setID].leftTop < cont[controller].vars[var].low)
    cont[controller].vars[var].sets[setID].leftTop = cont[controller].vars[var].low;
  if(cont[controller].vars[var].sets[setID].rightTop > cont[controller].vars[var].low)
    cont[controller].vars[var].sets[setID].rightTop = cont[controller].vars[var].high;
  if(cont[controller].vars[var].sets[setID].leftBase < cont[controller].vars[var].low)
    cont[controller].vars[var].sets[setID].leftBase = cont[controller].vars[var].low;
  if(cont[controller].vars[var].sets[setID].rightBase > cont[controller].vars[var].high)
    cont[controller].vars[var].sets[setID].rightBase = cont[controller].vars[var].high;
}

void MutateRule(int controller, int ruleID) {
  int mut = GetRandInt(0,0);
  cont[controller].mutations++;

  switch(mut){
    case 0: //swap the output
      do {
        random = GetRandInt(0, NUM_SETS);
        if(random != cont[controller].rules[ruleID].outputset) {
        cont[controller].rules[ruleID].outputset = random;
      }
  } while(1);
      break;
    default:
      return;
      break;
  }
}