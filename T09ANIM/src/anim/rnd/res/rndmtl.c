#include "anim/rnd/rnd.h"
#include "anim/anim.h"
#include <stdio.h>
#include <time.h>

me3MATERIAL ME3_RndMaterials[ME3_MAX_MATERIALS];
INT ME3_RndMaterialsSize = 0;

me3MATERIAL ME3_RndMtlGetDef( VOID )
{
  me3MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.2, 0.2, 0.2},
    {0.08, 0.08, 0.08},
    5, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };
  return def_mtl;
}

INT ME3_RndMtlAdd( me3MATERIAL *Mtl )
{
  if (ME3_RndMaterialsSize >= ME3_MAX_MATERIALS)
    return 0;
  ME3_RndMaterials[ME3_RndMaterialsSize] = *Mtl;
  return ME3_RndMaterialsSize++;
}

VOID ME3_RndMtlInit( VOID )
{
  me3MATERIAL def = ME3_RndMtlGetDef();

  ME3_RndMaterialsSize = 0;
  ME3_RndMtlAdd(&def);
}

VOID ME3_RndMtlClose( VOID )
{
  ME3_RndMaterialsSize = 0;
}


INT ME3_RndMtlApply( INT MtlNo )
{
  INT i;
  INT prg;
  me3MATERIAL *mtl;
  INT loc;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= ME3_RndMaterialsSize)
    MtlNo = 0;
  mtl = &ME3_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= ME3_RndShadersSize)
    prg = 0;
  prg = ME3_RndShaders[prg].ProgId;

  if (prg == 0)
    return 0;

  glUseProgram(prg);

  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);

  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";
    /*CHAR tnamew[] = "Texture0W";
    CHAR tnameh[] = "Texture0H";*/
    BOOL IsTex = FALSE;
 
    tname[9] = '0' + i;
    /*tnamew[7] = '0' + i;
    tnameh[7] = '0' + i;*/
    
    if (mtl->Tex[i] != -1 && mtl->Tex[i] > 0 && mtl->Tex[i] < ME3_MAX_TEXTURES)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, ME3_RndTextures[mtl->Tex[i]].TexId);  
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
    /*if ((loc = glGetUniformLocation(prg, tnamew)) != -1)
      glUniform1f(loc, VG4_RndTextures[mtl->Tex[i]].W);
    if ((loc = glGetUniformLocation(prg, tnameh)) != -1)
      glUniform1f(loc, VG4_RndTextures[mtl->Tex[i]].H);*/ 
  }  
  return prg;
}

me3MATERIAL * ME3_RndMtlGet( INT No )
{
  return &ME3_RndMaterials[No];
}
