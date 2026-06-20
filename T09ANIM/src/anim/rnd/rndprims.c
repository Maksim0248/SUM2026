/* FILE NAME: rndprims.c
 * PURPOSE: Functions for primitives.
 * PROGRAMMER: ME3
 * DATE: 19.06.2026
 */

#include "rnd.h"
#include "anim/anim.h"
#include <string.h>
#include <stdio.h>

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       me3PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL ME3_RndPrimsCreate( me3PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(me3PRIMS));
  if ((Prs->Prims = malloc(NumOfPrims * sizeof(me3PRIM))) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, NumOfPrims * sizeof(me3PRIM));
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'ME3_RndPrimsCreate' function */


/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       vg4PRIMS *Prs;
 * RETURNS: None.
 */
VOID ME3_RndPrimsFree( me3PRIMS *Prs )
{
  INT i;

  if (Prs->Prims != NULL)
  {
    for (i = 0; i < Prs->NumOfPrims; i++)
      ME3_RndPrimFree(&Prs->Prims[i]);
    free(Prs->Prims);
  }
  memset(Prs, 0, sizeof(me3PRIMS));
} /* End of 'VG4_RndPrimsFree' function */

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       vg4PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID ME3_RndPrimsDraw( me3PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->Trans, World);
 
  /* Draw all nontransparent primitives */ 
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (ME3_RndMtlGet(Prs->Prims[i].MtlNo)->Trans == 1)
      ME3_RndPrimDraw(&Prs->Prims[i], m);
 
  /* Draw all transparent primitives with front face culling */ 
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (ME3_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      ME3_RndPrimDraw(&Prs->Prims[i], m);
 
 
  /* Draw all transparent primitives with back face culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (ME3_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      ME3_RndPrimDraw(&Prs->Prims[i], m);
  glDisable(GL_CULL_FACE);
} /* End of 'VG4_RndPrimsDraw' function */


/* Load primitives from '*.G3DM' file function.
 * ARGUMENTS:
 *   - pointer to primitives to create:
 *       vg4PRIMS *Prs;
 *   - '*.G3DM' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ME3_RndPrimsLoad( me3PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen;
  UINT p, m, t;
  BYTE *mem, *ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  INT i;
  struct tagG3DM_MATERIAL
  {
    CHAR Name[300]; /* Material name */
    /* Illumination coefficients */
    VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
    FLT Ph;             /* Phong power coefficient – shininess */
    FLT Trans;          /* Transparency factor */
    DWORD Tex[8];       /* Texture references 
                        * (8 time: texture number in G3DM file, -1 if no texture) */
    /* Shader information */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;       /* Shader number (uses after load into memory) */
  } *mtls;
  //MATR trans_m = ME3_RndPrimsLoadTransform, trans_minv = MatrTranspose(MatrInverse(trans_m));
 
  //ME3_RndPrimsLoadTransform = MatrIdentity();
 
  /* Read whole file to memory */
  memset(Prs, 0, sizeof(me3PRIMS));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
 
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  rewind(F);
  fread(mem, 1, flen, F);
  fclose(F);
 
  ptr = mem;
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;
 
  if (!ME3_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
 
  /* Primitives */
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;
    DWORD MtlNo;
    me3VERTEX *V;
    INT *Ind;
    UINT i;
 
    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    V = (me3VERTEX *)ptr;
    ptr += sizeof(me3VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;
 
    /*for (i = 0; i < NumOfVertexes; i++)
    {
      V[i].P = PointTransform(V[i].P, trans_m);
      V[i].N = VectorTransform(V[i].N, trans_minv);
    }*/
 
    ME3_RndPrimCreate(&Prs->Prims[p], ME3_RND_PRIM_TRIMESH, V, NumOfVertexes, Ind, NumOfFacetIndexes);
    Prs->Prims[p].MtlNo = ME3_RndMaterialsSize + MtlNo;
  }
 
  /* Materials */
  mtls = (struct tagG3DM_MATERIAL *)ptr;
  ptr += sizeof(struct tagG3DM_MATERIAL) * NumOfMaterials;
  for (m = 0; m < NumOfMaterials; m++)
  {
    me3MATERIAL mtl = ME3_RndMtlGetDef();
 
    mtl.Ka = mtls[m].Ka;
    //mtl.Ka = VecSet(1, 0, 1);
    mtl.Kd = mtls[m].Kd;
    mtl.Ks = mtls[m].Ks;
    mtl.Ph = mtls[m].Ph;
    mtl.Trans = mtls[m].Trans;
    for (t = 0; t < 8; t++)
      if (mtls[m].Tex[t] != -1 && mtls[m].Tex[t] >= 0 && mtls[m].Tex[t] < NumOfTextures)
        mtl.Tex[t] = ME3_RndTexturesSize + mtls[m].Tex[t];
    ME3_RndMtlAdd(&mtl);
  }
 
  /* Textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR *Name = (CHAR *)ptr;
    DWORD W, H, C;
 
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    ME3_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }
  free(mem);
  return TRUE;
} /* End of 'VG4_RndPrimsDraw' function */

/* END OF 'rndprims.c' FILE */