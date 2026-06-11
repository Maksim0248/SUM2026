/* FILE NAME: me3UNIT.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "anim.h"

/* me3UNIT initialization function.
 * ARGUMENTS:
 *   - self-pointer to me3UNIT object:
 *       me3UNIT *Uni;
 *   - animation context:
 *       me3ANIM *Ani;
 * RETURNS: None.
 */
static VOID ME3_UnitInit( me3UNIT *Uni, me3ANIM *Ani )
{
} /* End of 'ME3_UnitInit' function */
 
/* me3UNIT deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to me3UNIT object:
 *       me3UNIT *Uni;
 *   - animation context:
 *       me3ANIM *Ani;
 * RETURNS: None.
 */
static VOID ME3_UnitClose( me3UNIT *Uni, me3ANIM *Ani )
{
} /* End of 'ME3_UnitClose' function */
 
/* me3UNIT inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to me3UNIT object:
 *       me3UNIT *Uni;
 *   - animation context:
 *       me3ANIM *Ani;
 * RETURNS: None.
 */
static VOID ME3_UnitResponse( me3UNIT *Uni, me3ANIM *Ani )
{
} /* End of 'ME3_UnitResponse' function */
 
/* me3UNIT render function.
 * ARGUMENTS:
 *   - self-pointer to me3UNIT object:
 *       me3UNIT *Uni;
 *   - animation context:
 *       me3ANIM *Ani;
 * RETURNS: None.
 */
static VOID ME3_UnitRender( me3UNIT *Uni, me3ANIM *Ani )
{
} /* End of 'ME3_UnitRender' function */

me3UNIT * ME3_AnimUnitCreate( INT Size )
{
  me3UNIT *Uni;
 
  /* Memory allocation */
  if (Size < sizeof(me3UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
 
  /* Setup me3UNIT methods */
  Uni->Init = ME3_UnitInit;
  Uni->Close = ME3_UnitClose;
  Uni->Response = ME3_UnitResponse;
  Uni->Render = ME3_UnitRender;
 
  return Uni;
} /* End of 'ME3_AnimUnitCreate' function */

