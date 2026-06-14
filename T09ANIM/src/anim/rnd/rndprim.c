/* FILE NAME: rndprim.c
 * PURPOSE: Functions for primitives.
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "rnd.h"
#include "anim/anim.h"
#include <string.h>
#include <stdio.h>

VOID ME3_RndPrimFree( me3PRIM *Pr )
{
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->VBuf);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(me3PRIM));
}

/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       me3PRIM *Pr;
 *   - primitive type:
 *       me3PRIM_TYPE Type;
 *   - vertex attributes array:
 *       me3VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID ME3_RndPrimCreate( me3PRIM *Pr, me3PRIM_TYPE Type, me3VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  memset(Pr, 0, sizeof(me3PRIM));
  Pr->Trans = MatrIdentity();
  Pr->Type = Type;
 
  glGenVertexArrays(1, &Pr->VA); /* create place in memory*/
 
  /* Vertex data */
  if (V != NULL && NoofV != 0)
  {
    glBindVertexArray(Pr->VA); /*active*/
    glGenBuffers(1, &Pr->VBuf); /* create place in memory*/
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf); /*active*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(me3VERTEX) * NoofV, V, GL_STATIC_DRAW); /* V: memory --> video memory*/ /*GL_STATIC_DRAW: V - static*/
    /* select vertex array*/
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(me3VERTEX), (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(me3VERTEX), (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(me3VERTEX), (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(me3VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */
    /*on*/
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
 
    glBindVertexArray(0);/*off*/
 
    /* Obtain BB */
    if (NoofV > 0)
    {
      INT i;
 
      Pr->MinBB = Pr->MaxBB = V[0].P;
      for (i = 1; i < NoofV; i++)
      {
        Pr->MinBB = VecMinVec(Pr->MinBB, V[i].P);
        Pr->MaxBB = VecMaxVec(Pr->MaxBB, V[i].P);
      }
    }
  }
  /* Index data */
  if (Ind != NULL && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf); /* activate*/
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Ind, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); /*deactivate*/
 
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;
} /* End of 'ME3_RndPrimCreate' function */


/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive to be draw:
 *       me3PRIM *Pr;
 *   - transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID ME3_RndPrimDraw( me3PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr(MatrMulMatr(Pr->Trans, World), ME3_RndMatrVP);
  INT prim_type =
    Pr->Type == ME3_RND_PRIM_LINES ? GL_LINES :
    Pr->Type == ME3_RND_PRIM_TRIMESH ? GL_TRIANGLES :
    GL_POINTS;
  UINT ProgId = ME3_RndProgId;
  INT loc;
 
  glUseProgram(ProgId);
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, ME3_Anim.Time);
  //glLoadMatrixf(wvp.A[0]);
 
  glBindVertexArray(Pr->VA);/*activate*/
  if (Pr->IBuf == 0)
    glDrawArrays(prim_type, 0, Pr->NumOfElements);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);/*activate*/
    glDrawElements(prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
} /* End of 'me3_RndPrimDraw' function */


BOOL ME3_RndPrimCreateSphere( me3PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;
  INT size;
  me3VERTEX *V;
  INT *Ind;
  VEC L = VecNormalize(VecSet(1, 1, 1));

  memset(Pr, 0, sizeof(me3PRIM));
  size = sizeof(me3VERTEX) * W * H + sizeof(INT) * (H - 1) * (W - 1) * 2 * 3;
 
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + W * H);
 

  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
    {
      V[k].C.X = (FLT)rand() / RAND_MAX;/*R*/
      V[k].C.Y = (FLT)rand() / RAND_MAX;/*G*/
      V[k].C.Z = (FLT)rand() / RAND_MAX;/*B*/

      V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));
    }
 
  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Ind[k++] = i * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j;
      /* top-right */
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j + 1;
    }

  ME3_RndPrimCreate(Pr, ME3_RND_PRIM_TRIMESH, V, W * H, Ind, (H - 1) * (W - 1) * 2 * 3);
  free(V);
  return TRUE;
} /* End of 'ME3_RndPrimCreateSphere' function */






BOOL ME3_RndPrimLoad( me3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0, i;
  static CHAR Buf[3000];
  INT size;
  me3VERTEX *V;
  INT *Ind;
  VEC L = VecNormalize(VecSet(1, 1, 1));

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

  size = sizeof(me3VERTEX) * nv + sizeof(INT) * nf * 3;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);

  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;

  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
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
 
            Ind[nf++] = n1;
            Ind[nf++] = n2;
            Ind[nf++] = n3;
 
            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
  /* добавить автонормали к каждой вершине */
  for (i = 0; i < nv; i++)
  {
    V[i].C = VecSet4(0.21 * Rnd1(), 0.11 * Rnd1(), 0.11 * Rnd1(), 1);

  }
  /*for (i = 0; i < nv; i++)
  {
    FLT nl = VecDotVec(L, V[i].N);
    if (nl < 0.2)
      nl = 0.2;
    V[i].C = VecSet4(0.67 * nl, 0.52 * nl, 0.47 * nl, 1);

  }*/
  ME3_RndPrimCreate(Pr, ME3_RND_PRIM_TRIMESH, V, nv, Ind, nf); 
  free(V);
  return TRUE;
} /* End of 'ME3_RndPrimLoad' function */
