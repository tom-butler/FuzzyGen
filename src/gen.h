#ifndef GEN_H
#define GEN_H
//options-------------------------------------------

//genetic
static int POP = 100;                         //-p
static int ANCESTOR = POP/2;                  //-a
static int NUM_VARS = 4;                      //-c
static int NUM_SETS = 3;                      //-s
static int NUM_RULES = NUM_VARS * NUM_SETS;
static int VARIANCE = 3;                      //-r
static int HEIGHT = 1;                        //-h
static float MUT_CHANCE = 0.3f;               //-m

//sim
static int START_FUEL = 1000;                 //-f
static int THRUST_MAX = 50;                   //-t
static int TERMINAL_VELOCITY = 50;            //-v
static int FORCE = 5;                         //-f
static int CRASH_SPEED = 5;                   //-x

int random;
#endif