/* Eremeev Maksim, 09-3, 03.06.2026 */

typedef DOUBLE DBL;

#define MAX 10
DBL A[MAX][MAX];
INT N;
INT k;

INT P[MAX];
//INT AllP[MAX!]
BOLL IsParity;


/*INT Factorial( int a )
{
  INT n;

  if (a <= 1)
    return ;

  return a * Factorial(a - 1);
}*/
BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
 
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

  if (Pos == MAX)
  {
    Det();
  }
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

INT Det( VOID )
{
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity * 2 - 1);
}

VOID main ( VOID )
{
  LoadMatrix("IN.TXT");

  FILE *F;
  
  F = fopen("PEREST.TXT", "a");/* a - write to end of file*/
  if (F == NULL)
    return;



}