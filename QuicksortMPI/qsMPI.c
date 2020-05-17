#include "qsMPI.h"

void parQSort(int *local_data,
              int *rcvBuf,
              int nLoc,
              int nRcv,
              MPI_Comm *comm,
              const int strat)
{
    int size, rank, locPiv, gPiv, pivInd = 0;
    int i, targRank, sendCount, sendDisp, rcvCount;
    MPI_Comm newComm;
    MPI_Status status;

    MPI_Comm_size(*comm, &size);
    MPI_Comm_rank(*comm, &rank);

    // find local pivot value
    locPiv = local_data[nLoc/2];

    gPiv = findPiv(locPiv, size, rank, strat, &comm);

    if (size = 1) // if size of communicator is 0, do nothing
    {
        return;
    }
    else
    {
        // Find largest element smaller than pivot
        while (local_data[pivInd] < gPiv)
        {
            pivInd++;
        }

        // Determine indicies and send counts
        if (rank > size/2)
        {
            targRank = rank - size/2;
            sendDisp = pivInd + 1;
            sendCount = nLoc - sendDisp;
        }
        else
        {
            targRank = rank + size/2;
            sendDisp = 0;
            sendCount = pivInd + 1;
        }
        
        MPI_Sendrecv(&sendCount, 1, MPI_INT, targRank, rank + 1,
                     &rcvCount, 1, MPI_INT, targRank, targRank + 1, *comm, &status);

        MPI_Sendrecv(&local_data[sendDisp], sendCount, MPI_INT, targRank, rank,
                     &rcvBuf[0], rcvCount, MPI_INT, targRank, targRank, *comm, &status);

        mergeArrays(&local_data, &nLoc, sendDisp, sendCount, &rcvBuf, rcvCount);

        /* Split communicator into two new communicators */    
        /* then recursively call parQSort again with new comm */

    }

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

void mergeArrays(int **local_data,
                 int *nLoc,
                 int sendDisp,
                 int sendCount,
                 int **rcvBuf,
                 int rcvCount)
{
    int i;
    if (sendDisp == 0)
    {
        int *tmp;   
        for (i = 0; i < *nLoc-sendCount; i++)
        {
            *rcvBuf[i+rcvCount] = *local_data[i+sendCount];
        }

        *nLoc = rcvCount + *nLoc - sendCount;
        tmp = *local_data;
        *local_data = *rcvBuf;
        *rcvBuf = tmp;
    }
    else
    {
        for (i = 0; i < rcvCount; i++)
        {
            *local_data[i+sendDisp] = *rcvBuf[i];
        }
        *nLoc = rcvCount + sendDisp;
    }
    
    qsort((void *) *local_data, *nLoc, sizeof(int), qsComp);

    return;
}