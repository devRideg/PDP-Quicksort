#include "qsAlgorithm.h"
#include <mpi.h>

void parQSort(int **local_data,
              int *rcvBuf,
              int **tmp_array,
              int nLoc,
              int nRcv,
              MPI_Comm *comm,
              const int strat);

int findPiv(int locPiv,
            int size,
            int rank,
            int start,
            MPI_Comm *comm);

void mergeArrays(int **local_data,
                 int *nLoc,
                 int sendDisp,
                 int sendCount,
                 int *rcvBuf,
                 int rcvCount,
                 int **tmp_array);