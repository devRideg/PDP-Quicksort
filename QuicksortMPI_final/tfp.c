#include "tfp.h"

int main(int argc, char *argv[])
{
  int locPiv, gPiv, size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm comm = MPI_COMM_WORLD;

  srand(time(0)+7*rank);

  locPiv = 3*size + rand() % (2*size);

  gPiv = findPiv(locPiv, 2, &comm);

  printf("locPiv = %d, gpiv = %d, pID:%d\n", locPiv, gPiv, rank);
  MPI_Finalize();
  return 0;
}
