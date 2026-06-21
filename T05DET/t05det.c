/* Eremeev Maksim, 09-3, 03.06.2026 */

#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef DOUBLE DBL;

#define MAX 10

DBL A[MAX][MAX];
INT N;

INT P[MAX];
BOOL IsParity;
DBL Det;

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;
  
  *A = *B;
  *B = tmp;
}

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
}


VOID Go( INT Pos )
{

  INT i;

  if (Pos == N)
  {
    DBL prod = 1;

    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity * 2 - 1);
  }
  else
  {
    for (i = Pos; i < N; i++)
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


VOID main ( VOID )
{
  FILE *F;
  INT i;

  LoadMatrix("IN.TXT");
  for (i = 0; i < N; i++)
    P[i] = i;
  IsParity = TRUE;
  Go(0);
  F = fopen("PEREST.TXT", "a");/* a - write to end of file*/
  if (F == NULL)
    return;
  fprintf(F, "%lf", Det);
  fclose(F);
}