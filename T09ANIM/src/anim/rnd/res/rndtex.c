#include "anim/rnd/rnd.h"
#include <stdio.h>

me3TEXTURE ME3_RndTextures[ME3_MAX_TEXTURES]; /* Array of textures */
INT ME3_RndTexturesSize = 0;                      /* Textures array store size */

VOID ME3_RndTexInit( VOID )
{

}

VOID ME3_RndTexClose( VOID )
{
}


INT ME3_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;
 
  if (ME3_RndTexturesSize >= ME3_MAX_TEXTURES)
    return -1;
 
  /* Setup OpenGL texture */
  glGenTextures(1, &ME3_RndTextures[ME3_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, ME3_RndTextures[ME3_RndTexturesSize].TexId);
 
 
  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;
 
  glTexStorage2D(GL_TEXTURE_2D, mips, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, W, H);
 
  if (Bits != NULL)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
                    C == 4 ? GL_BGRA : C == 3 ? GL_BGR : GL_RED, GL_UNSIGNED_BYTE, Bits);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
  return ME3_RndTexturesSize++;
} /* End of 'VG4_RndTexAddImg' function */

INT ME3_RndTexAdd( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  return -1;
}