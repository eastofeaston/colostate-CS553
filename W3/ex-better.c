#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void foo(int N)
{
  int i;
  int sum = 0;
  if (N > 1000)
    for (i = 0; i < N; i++)
    sum += i;
  printf ("result: %d\n", sum);
}

void bar(float* A, float* B, int N)
{
  int i, j;
  float sum = 0;
  for (i = 0; i < N; i++)
    sum += A[i];
  for (j = 0; j < N; j++)
    B[j] += sum;
  printf ("result: %.3f\n", (N >= 1000) ? sum : 0);
}

int main(int argc, char** argv)
{
  /* int N = atoi(argv[1]); */
  /* int N = 16384; */
  int N = 20000;
  /* int N = 20001; */
  float* A = malloc(N*sizeof(float));
  float* B = malloc(N*sizeof(float));
  bar (A, B, N);
  /* printf ("result: %.3f\n", B[N-1]); */
  return 0;
}
