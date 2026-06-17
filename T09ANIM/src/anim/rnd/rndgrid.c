/* FILE NAME: rndgrid.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#include "rnd.h"

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ME3GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL ME3_RndGridCreate( me3GRID *G, INT W, INT H )
{
  INT i, j;

  G->V = malloc(sizeof(me3VERTEX) * W * H);
  memset(G->V, 0, sizeof(me3VERTEX) * W * H);

  for (i = 0; i < H; i++)
    for (j = 0; j < W; j++)
    {
      G->V[i * W + j].N = VecSet(0, 1, 0);
      G->V[i * W + j].T.X = j / (W - 1.0);
      G->V[i * W + j].T.Y = i / (H - 1.0);
    }
  G->H = H;
  G->W = W;

  return TRUE;
}

VOID ME3_RndGridFree( me3GRID *G )
{
  free(G->V);
  memset(G, 0, sizeof(me3GRID));
}
 
/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       me3PRIM *Pr;
 *   - grid data:
 *       me3GRID *G;
 * RETURNS: None.
 */
BOOL ME3_RndPrimFromGrid( me3PRIM *Pr, me3GRID *G )
{
  INT i, j, k, num_of_indices;
  INT *Ind;
  
  memset(Pr, 0, sizeof(me3PRIM));
 
  num_of_indices = (G->W * 2 + 1) * (G->H - 1) - 1;
  if ((Ind = malloc(sizeof(INT) * ((G->H - 1) * (G->W * 2 + 1) - 1))) == NULL)
    return FALSE;
 
  for (k = 0, i = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = (i + 1) * G->W + j;
      Ind[k++] = i * G->W + j;
    }
    if (i != G->H - 2)
      Ind[k++] = -1;
  }
  ME3_RndPrimCreate(Pr, ME3_RND_PRIM_TRISTRIP, G->V, G->W * G->H, Ind, (G->H - 1) * (G->W * 2 + 1) - 1);
  return TRUE;
}

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       me3GRID *G;
 * RETURNS: None.
 */
/*VOID ME3_RndGridAutoNormals( me3GRID *G )
{
  INT i, j;

  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = VecSet(0, 0, 0);
 
  for (i = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++)
    {
      me3VERTEX
        *P00 = G->V + i * G->W + j,
        *P01 = G->V + i * G->W + j + 1,
        *P10 = G->V + (i + 1) * G->W + j,
        *P11 = G->V + (i + 1) * G->W + j + 1;
      VEC N;
 
      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);
 
      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }
}*/

/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       me3GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ME3_RndGridCreateSphere( me3GRID *G, FLT R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;

  if (!ME3_RndGridCreate(G, W, H))
    return FALSE;

  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
    {
      G->V[k].C = VecSet4(1, 1, 0, 1);
      G->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                           R * cos(theta),
                           R * sin(theta) * cos(phi));
    }
  return TRUE;
}