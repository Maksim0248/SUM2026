#include "map.h"

me3WALL ME3_MapWalls[17] =
{
  {-10, 10, 9.6, 9.8},
  {9.9, 10.1, 0, 9.7},
  {9.9, 10.1, -9.8, -3.5},
  {-10, 10, -9.9, 9.8},
  {-10.1, -9.9, -9.8, 9.7},
  {-0.1, 0.1, 6.7, 9.7},
  {3.4, 6.8, 6.6, 6.8},
  {-6.8, -3.4, 6.6, 6.8},
  {6.8, 10, 3.4, 3.6},
  {6.7, 6.9, -3.5, 3.5},
  {6.8, 10, -6.8, -6.6},
  {-3.4, 6.8, -3.6, -3.4},
  {-6.8, 3.4, -6.8, 6.6},
  {3.3, 3.5, -6.7, 6.7},
  {-3.4, 3.4, -0.1, 0.1},
  {-3.5, -3.3, 0, 3.5},
  {-6.7, -6.9, -6.7, 6.7}
};

BOOL Camin1Wall(FLT x, FLT z, me3WALL *w)
{
  if ((x > w->MinX) && (x < w->MaxX) && (z > w->MinZ) && (z < w->MaxZ))
    return TRUE;
  return FALSE;
}

BOOL ME3_CaminWall(FLT x, FLT z)
{
  int i;

  for (i = 0; i < 17; i++)
    if (Camin1Wall(x, z, &ME3_MapWalls[i]))
      return TRUE;

  return FALSE;
}