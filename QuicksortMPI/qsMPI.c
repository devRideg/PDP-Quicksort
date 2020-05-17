#include "qsMPI.h"

void parQSort(int *local_data,
              int *rcvBuf,
              int nLoc,
              int nRcv,
              MPI_Comm *comm,
              const int strat)
{
    int size, rank, locPiv, gPiv;
    int i;

    MPI_Comm_size(*comm, &size);
    MPI_Comm_rank(*comm, &rank);

    // find local pivot value
    locPiv = local_data[nLoc/2];

    gPiv = findPiv(locPiv, size, rank, strat, &comm);

    return;
}

int findPiv(int locPiv,
            int size,
            int rank,
            int start,
            MPI_Comm *comm)
{
    int Pivs[size], result;

    MPI_Allgather(&locPiv, 1, MPI_INT, &Pivs[0], size, MPI_INT, *comm);

    if (strat == 1)             // choose median of rank 0
    {
        result = Pivs[0];
    }
    else if (start == 2)        // choose the median of the medians
    {
        qsort((void *)Pivs, size, sizeof(int), qsComp);
        result = Pivs[size/2];
    }
    else if (start == 3)        // choose the mean of the medians
    {
        for (i = 0; i < size; i++)
        {
            result += Pivs[i];
        }
        result = result/size;
    }

    return result;
}