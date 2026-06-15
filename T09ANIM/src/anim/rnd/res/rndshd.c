#include "anim/rnd/rnd.h"
#include <stdio.h>
#include <time.h>


UINT ME3_RndProgId;

me3SHADER ME3_RndShaders[ME3_MAX_SHADERS];

/* Save log to file function.
 * ARGUMENTS:
 *   - shader prefix:
 *       CHAR *FileNamePrefix;
 *   - shader name:
 *       CHAR *ShaderName;
 *   - error text:
 *       CHAR *Text;
 * RETURNS: None.
 */
static VOID ME3_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;
 
  if ((F = fopen("bin/shaders/shd{30}ME3.log", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
} 
 
/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */

static CHAR * ME3_RndLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;
 
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;
 
  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
 
  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);
 
  /* Load text */
  rewind(F);
  fread(txt, 1, flen, F);
 
  fclose(F);
  return txt;
} /* End of 'ME3_RndLoadTextFromFile' function */


/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (UINT) load shader program Id.
 */
static UINT ME3_RndShdLoad( CHAR *FileNamePrefix )
{
  CHAR *txt;
  struct
  {
    CHAR *Name;  /* Shader name (e.g. "VERT") */
    INT Type;    /* Shader type (e.g. GL_VERTEX_SHADER) */
    INT Id;      /* Created shader Id */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER, 0},
    {"FRAG", GL_FRAGMENT_SHADER, 0},
  };
  INT res, i, NoofS = sizeof(shd) / sizeof(shd[0]);
  UINT prg;
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];
 
  for (i = 0; i < NoofS; i++)
  {
    /* Build shader name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shd[i].Name);
 
    /* Load shader text from file */
    txt = ME3_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      ME3_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    shd[i].Id = glCreateShader(shd[i].Type);
    if (shd[i].Id == 0)
    {
      free(txt);
      ME3_RndShdLog(FileNamePrefix, shd[i].Name, "Error shader create");
      is_ok = FALSE;
      break;
    }
    /* Send shader source text to OpenGL */
    glShaderSource(shd[i].Id, 1, &txt, NULL); /*put text in videocard*/
    free(txt);
 
    /* Compile shader */
    glCompileShader(shd[i].Id);
 
    /* Errors handle */
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      ME3_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }
 
  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      ME3_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shader programs */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link program */
      glLinkProgram(prg);
      /* Errors handle */
      glGetProgramiv(shd[i].Id, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
        ME3_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
 
  /* Handle errors */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'ME3_RndShdLoad' function */
 
/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT ProgId;
 * RETUNS: None.
 */
static VOID ME3_RndShdFree( UINT ProgId )
{
  INT shds[5], n, i;
 
  if (ProgId == 0 || !glIsProgram(ProgId))
    return;
 
  glGetAttachedShaders(ProgId, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(ProgId, shds[i]);
      glDeleteShader(shds[i]);
    }
  glDeleteProgram(ProgId);
} /* End of 'ME3_RndShdFree' function */


INT ME3_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;
 
  for (i = 0; i < ME3_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, ME3_RndShaders[i].Name) == 0)
      return i;
  if (ME3_RndShadersSize >= ME3_MAX_SHADERS)
    return 0;
  strncpy(ME3_RndShaders[ME3_RndShadersSize].Name, ShaderFileNamePrefix, ME3_STR_MAX - 1);
  ME3_RndShaders[ME3_RndShadersSize].ProgId = ME3_RndShdLoad(ShaderFileNamePrefix);
  return ME3_RndShadersSize++;
} /* End of 'ME3_RndShdAdd' function */
 
/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */

VOID ME3_RndShdUpdate( VOID )
{
  INT i, t = clock();
  static INT old_time;
  
  if (t - old_time > 2 * CLOCKS_PER_SEC)
  {
    for (i = 0; i < ME3_RndShadersSize; i++)
    {
      ME3_RndShdFree(ME3_RndShaders[i].ProgId);
      ME3_RndShaders[i].ProgId = ME3_RndShdLoad(ME3_RndShaders[i].Name);
      old_time = t
    }
  }
} /* End of 'ME3_RndShdUpdate' function */

VOID ME3_RndShdInit( VOID )
{
  ME3_RndProgId = ME3_RndShdAdd("bin/shaders/default");
} /* End of 'ME3_RndShdInit' function */
 

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ME3_RndShdClose( VOID )
{
  INT i;
 
  for (i = 0; i < ME3_RndShadersSize; i++)
    ME3_RndShdFree(ME3_RndShaders[i].ProgId);
  ME3_RndShadersSize = 0;
} /* End of 'ME3_RndShdClose' function */