#ifndef GEN_H
#define GEN_H



//options-------------------------------------------

//genetic
static int POP = 100;                         //-p
static int GENERATIONS = 100;                 //-g
static int ANCESTOR = POP/2;                  //-a
static int VARIANCE = 3;                      //-r
static float MUT_CHANCE = 0.3f;               //-m

//fuzzy
static int NUM_VARS = 4;                      //-c
static int NUM_SETS = 3;                      //-s
static int NUM_RULES = NUM_VARS * NUM_SETS;
static int HEIGHT = 1;                        //-h

//sim
static int START_HEIGHT = 1000;               //-y
static int START_FUEL = 1000;                 //-f
static int START_VEL = 3;                     //-v
static int THRUST_MAX = 50;                   //-t
static int TERMINAL_VELOCITY = START_VEL * 10;
static int FORCE = 5;                         //-o
static int CRASH_SPEED = 5;                   //-x

//shared
static int random;

//functions
void InitGen();
void InitSystem(int argc, char *argv[]);
void InitControllers();
void GALoop();
void ScoreFitness();
#endif