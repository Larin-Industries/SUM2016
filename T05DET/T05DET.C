#include <stdio.h>
#include <conio.h>

#define MAX 30

double A[MAX][MAX], sum;
int P[MAX], N;
char Parity = 1, SaveParity;

void Swap( int *A, int *B )
{
  int tmp;
           
  tmp = *A;
  *A = *B;
  *B = tmp;
}

void Go( int Pos )
{
  int i, x;
  double prod = 1;

  if (Pos == N)
  {
    for (i = 0; i < N; i++)
      prod *= A[i][P[i]];

    if (Parity)
      sum += prod;
    else
      sum -= prod;

    return;
  }
  else
  {
    for (i = Pos; i < N; i++)
    {
      
      Swap(&P[Pos], &P[i]);
      if (Pos != i)
        Parity = !Parity;
      Go(Pos + 1);
      if (Pos != i)
        Parity = !Parity;    
      Swap(&P[Pos], &P[i]);
      /*
      Go(Pos + 1);

      SaveParity = Parity;
      for (i = Pos + 1; i < N; i++)
      {
        Swap(&P[Pos], &P[i]);
        Parity = !Parity;
        Go(Pos + 1);
      }

      x = P[Pos];

      for (i = Pos + 1; i < N; i++)
        P[i - 1] = P[i];
      P[N - 1] = x;

      Parity = SaveParity;
      */
    }
  }               
}

void LoadMatrix( char *FileName )
{
  FILE *F;
  int i, j;

  F = fopen(FileName, "r");
  if (F == NULL)
    return;

  fscanf(F, "%d", &N);

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
}

double EvalDeterm( char *FileName )
{
  int i;

  LoadMatrix(FileName);

  sum = 0;
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);

  return sum;
}
           
void main( void )
{
  int i;  
  char *M[] = 
  {
    "mat1.txt", "mat2.txt", "mat3.txt", "mat4.txt"
  };

  for (i = 0; i < N; i++)
    P[i] = i;

  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterm(M[i]));

  _getch();
}