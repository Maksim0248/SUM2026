/* FILE NAME: rndprim.c
 * PURPOSE: Functions for primitives.
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "rnd.h"
#include <string.h>
#include <stdio.h>

/* Primitives */
VOID ME3_RndPrimFree( me3PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(me3VERTEX));   /* <-- <string.h>, WinAPI, ZeroMemory */
}

BOOL ME3_RndPrimCreate( me3PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;
 
  memset(Pr, 0, sizeof(me3PRIM));
  size = sizeof(me3VERTEX) * NoofV + sizeof(INT) * NoofI;
 
  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);
  return TRUE;
}

VOID ME3_RndPrimDraw( me3PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr(MatrMulMatr(Pr->Trans, ME3_RndMatrVP), World);
  
  glLoadMatrixf(wvp.A[0]);

  /* Draw triangles by edges */
  glBegin(GL_TRIANGLES); /* begin draw triangles*/
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor4fv(&Pr->V[Pr->I[i]].C.X); /* set color of vertix */
    glVertex3fv(&Pr->V[Pr->I[i]].P.X); /* set coordinate of vertix*/
  }
  glEnd();
}

BOOL ME3_RndPrimCreateSphere( me3PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;
 
  if (!ME3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;
 
  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
    {
      Pr->V[k].C.X = (FLT)rand() / RAND_MAX;
      Pr->V[k].C.Y = (FLT)rand() / RAND_MAX;
      Pr->V[k].C.Z = (FLT)rand() / RAND_MAX;
      Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));
    }
 
  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
} /* End of 'ME3_RndPrimCreateSphere' function */



/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       me3PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */

BOOL ME3_RndPrimLoad( me3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0, i;
  static CHAR Buf[3000];
 
  memset(Pr, 0, sizeof(me3PRIM));
 
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
 
  /* Count vertices and indices */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';
 
      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
          n++;
        oldc = *ptr++;
      }
 
      nf += n - 2;
    }
  }
 
  if (!ME3_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }
 
  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n, n1, n2, n3;
      INT fvn = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';
 
      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
        {
          sscanf(ptr, "%d", &n);
          if (n > 0)
            n--;
          else
            if (n < 0)
              n = nv + n;
 
          if (fvn == 0)
            n1 = n;
          else if (fvn == 1)
            n2 = n;
          else
          {
            n3 = n;
 
            Pr->I[nf++] = n1;
            Pr->I[nf++] = n2;
            Pr->I[nf++] = n3;
 
            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
  for (i = 0; i < Pr->NumOfV; i++)
    Pr->V[i].C = VecSet4(1, 1, 1, 1); /*+ rnd0*/
  return TRUE;
} /* End of 'ME3_RndPrimLoad' function */
