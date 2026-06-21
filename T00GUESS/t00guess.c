/* Eremeev Maksim, 09-3, 01.06.2026 */

#include <windows.h>
#include <stdio.h>

void main( void )
{
  char s[100];
  int b = 100;
  int a = 0;
  int m;
  int flag = 1;
  int k;

  if (MessageBox(NULL, "Start?", "First question", MB_YESNO | MB_ICONQUESTION) == IDNO)
    return;
  while (flag == 1)
  {
    if (a > b)
    {
      MessageBox(NULL, "You are lier", "problem", MB_YESNOCANCEL | MB_ICONERROR);
      flag = 0;
      break;
    }

    m = (a + b) / 2;

    sprintf(s, "Your number > %i (cancel if =)", m);

    k = MessageBox(NULL, s, "question", MB_YESNOCANCEL | MB_ICONQUESTION);
    if (k == IDYES)
    {
      a = m + 1;
      flag = 1;
    }
    if (k == IDNO)
      {
        b = m - 1;
        flag = 1;
    }
    if (k == IDCANCEL)
    {
      flag = 0;
      sprintf(s, "Your number is %i", m);
      MessageBox(NULL, s, "Win", MB_YESNO | MB_ICONINFORMATION);
    }
  }
}