/* FILE NAME: u_tex.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>
#include <stdio.h>
#include "units.h"

typedef struct tagUNIT_TEX me3UNIT_TEX;
struct tagUNIT_TEX
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Pr;
  INT MtlNo, TexId[2];
};

static VOID ME3_UnitInit(me3UNIT_TEX *Uni, me3ANIM *Ani)
{
  me3VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{20, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 20, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{20, 20, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
  };
 
  FLT t[] =
  {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    1, 1, 1
  };
  FILE *F;
  me3MATERIAL mtl = ME3_RndMtlGetDef();

  strcpy(mtl.Name, "Texture");
  mtl.ShdNo = ME3_RndShdAdd("tex");  
  Uni->MtlNo = ME3_RndMtlAdd(&mtl);
  
  glGenTextures(2, Uni->TexId); /*create object of texture*/

  glBindTexture(GL_TEXTURE_2D, Uni->TexId[0]); /* bind GL_TEXTURE_2D <--> Uni->TexId*/
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGB, GL_FLOAT, t);/*translate pixels to videocard*/
 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  ME3_RndPrimCreate(&Uni->Pr, ME3_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
  Uni->Pr.MtlNo = Uni->MtlNo;

  if ((F = fopen("bin\\textures\\BRwall.g24", "rb")) != NULL)
  {
    INT w = 0, h = 0;
    VOID *mem;

    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);

    if ((mem = malloc(w * h * 3)) != NULL)
    {
      fread(mem, 3, w * h, F);

      glBindTexture(GL_TEXTURE_2D, Uni->TexId[1]); /* bind GL_TEXTURE_2D <--> Uni->TexId*/
      glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, mem);
 
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

     
      free(mem);
    }
    fclose(F);
  }
}

static VOID ME3_UnitResponse(me3UNIT_TEX *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_TEX *Uni, me3ANIM *Ani, me3PRIM *sph)
{

  //glActiveTexture(GL_TEXTURE0 + 1);
  //glBindTexture(GL_TEXTURE_2D, Uni->TexId[0]);  
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, Uni->TexId[1]);  

  ME3_RndPrimDraw(&Uni->Pr, MatrIdentity());
}

static VOID ME3_UnitClose(me3UNIT_TEX *Uni, me3ANIM *Ani)
{
  glDeleteTextures(1, Uni->TexId);
  ME3_RndPrimFree(&Uni->Pr);
}


me3UNIT * ME3_UnitCreateTex( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_TEX))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}