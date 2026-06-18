#include "anim/rnd/rnd.h"
#include <stdio.h>

me3TEXTURE ME3_RndTextures[ME3_MAX_TEXTURES]; /* Array of textures */
INT ME3_RndTexturesSize = 0;                      /* Textures array store size */

VOID ME3_RndTexInit( VOID )
{
  ME3_RndTexturesSize = 0;
}

VOID ME3_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < ME3_RndTexturesSize; i++)
    glDeleteTextures(1, &ME3_RndTextures[i].TexId);
  ME3_RndTexturesSize = 0;
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
  INT ret = -1;
  HBITMAP hBm;
  FILE *F;
 
  if ((hBm = LoadImage(NULL, Name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP Bm;
 
    GetObject(hBm, sizeof(BITMAP), &Bm);
    if (Bm.bmBitsPixel == 24 || Bm.bmBitsPixel == 32 || Bm.bmBitsPixel == 8)
      ret = ME3_RndTexAddImg(Name, Bm.bmWidth, Bm.bmHeight, Bm.bmBitsPixel >> 3, Bm.bmBits); 
    DeleteObject(hBm);
    return ret;
  }
  if ((F = fopen(Name, "rb")) != NULL)
  {
    INT w = 0, h = 0, flen, components = -1;
    VOID *mem;
 
    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
 
    fseek(F, 0, SEEK_END);
    flen = ftell(F);
    fseek(F, 4, SEEK_END);
 
    if (w * h * 4 + 4 == flen)
      components = 4;
    else if (w * h * 3 + 4 == flen)
      components = 3;
    else if (w * h * 1 + 4 == flen)
      components = 1;
 
    if (components != -1)
      if ((mem = malloc(w * h * components)) != NULL)
      {
        fread(mem, components, w * h, F);
        ret = ME3_RndTexAddImg(Name, w, h, components, mem); 
        free(mem);
      }
    fclose(F);
  }
  return ret;
} /* End of 'ME3_RndTexAddFromFile' function */