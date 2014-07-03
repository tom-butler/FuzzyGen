#ifndef GEN_H
#define GEN_H

#include <string>
//options-------------------------------------------
//system
static bool DEBUG = true;                    //-d

//genetic
static int POP = 10000;                         //-p
static int GENERATIONS = 10000;                 //-g
static int ANCESTOR = POP/2;                  //-a
static int VARIANCE = 3;                      //-r
static float MUT_CHANCE = 0.3f;               //-m
static int BEST = 0;

//fuzzy
static int NUM_INPUT = 2;
static int NUM_OUTPUT = 1;
static int NUM_VARS = 4;                      //-c
static int NUM_SETS = 3;                      //-s
static int NUM_RULES = 9;
static int HEIGHT = 1;                        //-h

//sim
static int START_HEIGHT = 100;               //-y
static int START_FUEL = 1000;                 //-f
static int START_VEL = 3;                     //-v
static int THRUST_MAX = 50;                   //-t
static int TERMINAL_VELOCITY = START_VEL * 10;
static int FORCE = 3;                         //-o
static int CRASH_SPEED = 5;                   //-x

//shared
static int random;

//functions
void Print(std::string error);
void InitGen();
void InitSystem(int argc, char *argv[]);
void InitControllers();
void GALoop();
void ScoreFitnesses();
#endif