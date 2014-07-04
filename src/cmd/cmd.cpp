#include "..\objects\gen.h"

int main(int argc, char *argv[])
{
  InitSystem(argc, argv);
  InitControllers();
  GALoop();
}