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

  if (MessageBox(NULL, "Start?", "First question", MB_YESNO | MB_ICONQUESTION) == IDNO)
    return;
  m = (a + b) / 2;
  while (flag != 0)
  {
    sprintf(s, "Your number > %i (cancel if =)", m);
    if (MessageBox(NULL, s, "Second question", MB_YESNOCANCEL | MB_ICONQUESTION) == IDYES)
    {
      m = (m + 1 + b) / 2;
      flag = 1;
    }
    else
    {
      if (MessageBox(NULL, s, "Second question", MB_YESNOCANCEL | MB_ICONQUESTION) == IDNO)
      {
        m = (a + (m - 1)) / 2;
        flag = 1;
      }
      else
      {
        if (MessageBox(NULL, s, "Second question", MB_YESNOCANCEL | MB_ICONQUESTION) == IDCANCEL)
        {
          flag = 0;
          sprintf(s, "Your number is %i", m);
          MessageBox(NULL, s, "Second question", MB_YESNO | MB_ICONQUESTION);
        }
      }
    }
  }
}