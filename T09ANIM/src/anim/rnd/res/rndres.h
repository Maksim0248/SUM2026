#ifndef __rndres_h_
#define __rndres_h_

#include "anim/rnd/rnd.h"
#include "def.h"

#define ME3_STR_MAX 500

VOID ME3_RndResInit( VOID );
VOID ME3_RndResClose( VOID );


/* Types*/
typedef struct tagme3SHADER
{
  CHAR Name[ME3_STR_MAX];
  UINT ProgId;
} me3SHADER;

typedef struct tagme3MATERIAL
{
  CHAR Name[ME3_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */

  FLT Trans;    /* Transparency factor */

  INT Tex[8];   /* Texture references from texture table (or -1) */

  INT ShdNo;    /* Shader number in shader table */
} me3MATERIAL;

typedef struct tagme3TEXTURE
{
  CHAR Name[ME3_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} me3TEXTURE;


/* Shaders */
#define ME3_MAX_SHADERS 30

extern me3SHADER ME3_RndShaders[ME3_MAX_SHADERS];
extern INT ME3_RndShadersSize;

VOID ME3_RndShdInit( VOID );
VOID ME3_RndShdClose( VOID );
VOID ME3_RndShdUpdate( VOID );
INT ME3_RndShdAdd( CHAR *ShaderFileNamePrefix );




/* Textures */
#define ME3_MAX_TEXTURES 300

extern me3TEXTURE ME3_RndTextures[ME3_MAX_TEXTURES]; /* Array of textures */
extern INT ME3_RndTexturesSize;                      /* Textures array store size */

VOID ME3_RndTexInit( VOID );
VOID ME3_RndTexClose( VOID );
INT VG4_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT ME3_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT ME3_RndTexAddFromFile( CHAR *FileName );


/* Materials */
#define ME3_MAX_MATERIALS 300

extern me3MATERIAL ME3_RndMaterials[ME3_MAX_MATERIALS];
extern INT ME3_RndMaterialsSize;

VOID ME3_RndMtlInit( VOID );
VOID ME3_RndMtlClose( VOID );
INT ME3_RndMtlAdd( me3MATERIAL *Mtl );
me3MATERIAL ME3_RndMtlGetDef( VOID );
UINT ME3_RndMtlApply( INT MtlNo );
me3MATERIAL * ME3_RndMtlGet( INT No );

#endif /*__rndres_h_*/