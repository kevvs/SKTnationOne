#include "inverse.h"

void main()
{
  double t1 = omp_get_wtime();
  C_Inverse bla;
  bla.input("input.txt");
  bla.calc();
  double t2 = omp_get_wtime();
  printf("TIME: %.6lf\n", (t2 - t1) * 1000.0f);
  getchar();
}