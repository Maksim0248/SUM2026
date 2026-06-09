/* FILE NAME: t04perem.c
 * PROGRAMMER: ME3
 * DATE: 06.06.2026
 */

#include <stdio.h>
#include <windows.h>

#define MAX 4
INT P[MAX];
BOOL IsParity = 0;

VOID Store( VOID )
{
  INT i;
  FILE *F;
  
  F = fopen("PEREST.TXT", "a");/* a - write to end of file*/
  if (F == NULL)
    return;
  for (i = 0; i < MAX; i++)
    fprintf(F, "%d%s", P[i], i < MAX - 1 ? " " : "");
  if (IsParity)
    fprintf(F, " - even");
  else
    fprintf(F, " - odd");

  fprintf(F, "\n");
  fclose(F);
}

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;
  
  *A = *B;
  *B = tmp;
}

VOID Go( INT Pos )
{

  INT i;

  if (Pos == MAX)
    Store();
  else
  {
    for (i = Pos; i < MAX; i++)
    {
      if (Pos != i)
        IsParity = !IsParity;
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      if (Pos != i)
        IsParity = !IsParity;
      Swap(&P[Pos], &P[i]);
    }
  }
}

VOID main(VOID)
{
  INT i;

  for (i = 0; i < MAX; i++)
    P[i] = i + 1;
  Go(0);
  return;
}