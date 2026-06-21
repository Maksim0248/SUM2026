#include "def.h"


typedef struct tagme3WALL
{
  float MinX, MaxX;
  float MinZ, MaxZ;
} me3WALL;

extern me3WALL ME3_MapWalls[17];
BOOL ME3_CaminWall(FLT x, FLT z);