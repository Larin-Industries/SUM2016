#include <stdio.h>
#include <conio.h>

#define N 4

int P[N], Numb = 1;
char Chet = 1;

long Fact()
{
  int i;
  long fact = 1;

  for (i = 0; i < N; i++)
    fact *= P[i];

  return fact;
}

void SavePerm( void )
{
  int i;
  FILE *F;

  F = fopen("Perm.log", "a");

  if (F == NULL)
    return;
  for (i = 0; i < N - 1; i++)
    fprintf(F, "%i, ", P[i]);
  fprintf(F, "%i;", P[N - 1]);

  if (Chet == 1)
    fprintf(F, " -- Parity");
  else
    fprintf(F, " -- Not parity");
  
  fprintf(F, "\n");
  fclose(F);
}

           
void Swap( int *A, int *B )
{
  int tmp;
  
  tmp = *A;
  *A = *B;
  *B = tmp;
}

void Go( int Pos, long Number, FILE *F )
{
  int i;

  if (Pos == N)
  {
    SavePerm();    
    printf("%f%%\r", (double)Numb / Number * 100.0);
    Numb++;
    return;
  }
  else
  {
    for (i = Pos; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      if (Pos != i)
        Chet = !Chet;
      Go(Pos + 1, Number, F);
      Swap(&P[Pos], &P[i]);
      if (Pos != i)
        Chet = !Chet;    
    }
  }
}

void main( void )
{
  char flag = 1;
  int i;
  FILE *F;  

  printf("Do you want to clear the file? [Y/N]:");
  while(flag)
  {
    switch(_getch())
    {
    case 'y':
      F = fopen("Perm.log", "w");
      if (F == NULL)
        return;
      flag = 0;
      break;
    default:
      flag = 0;
      break;
    }
  }

  F = fopen("Perm.log", "a");
 
  for (i = 0; i < N; i++)
    P[i] = i + 1;

  Go(0, Fact(), F);

  fprintf(F, "---------\n\r");
}