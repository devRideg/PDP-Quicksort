#include "qsMPI.h"

// main paralell Quick sort algorithm
void parQSort(int **local_data,
              int *rcvBuf,
              int **tmp_array,
              int nLoc,
              MPI_Comm *comm,
              const int strat)
{
    int size, rank, locPiv, gPiv, pivInd = 0, colour;
    int i, targRank, sendCount, sendDisp, rcvCount;
    MPI_Comm newComm;
    MPI_Status status;

    MPI_Comm_size(*comm, &size);
    MPI_Comm_rank(*comm, &rank);

    // find local pivot value
    locPiv = (*local_data)[nLoc/2];

    gPiv = findPiv(locPiv, size, rank, strat, comm);

    if (size = 1) // if size of communicator is 0, do nothing
    {
        return;
    }
    else
    {
        // Find largest element smaller than pivot
        while ((*local_data)[pivInd] < gPiv)
        {
            pivInd++;
        }

        // Determine indicies and send counts
        if (rank >= size/2)
        {
            targRank = rank - size/2;
            sendDisp = pivInd + 1;
            sendCount = nLoc - sendDisp;
            colour = 2;
        }
        else
        {
            targRank = rank + size/2;
            sendDisp = 0;
            sendCount = pivInd + 1;
            colour = 1;
        }
        
        MPI_Sendrecv(&sendCount, 1, MPI_INT, targRank, rank + 1,
                     &rcvCount, 1, MPI_INT, targRank, targRank + 1, *comm, &status);

        MPI_Sendrecv(&local_data[sendDisp], sendCount, MPI_INT, targRank, rank,
                     &rcvBuf[0], rcvCount, MPI_INT, targRank, targRank, *comm, &status);

        mergeArrays(local_data, &nLoc, sendDisp, sendCount, rcvBuf, rcvCount, tmp_array);

        /* Split communicator into two new communicators */    
        MPI_Comm_split(*comm, colour, rank, &newComm);

        /* then recursively call parQSort again with new comm */
        parQSort(local_data, rcvBuf, tmp_array, nLoc, &newComm, strat);
    }

    return;
}

// Find the pivot elemenmt based on given strategy
int findPiv(int locPiv,
            int size,
            int rank,
            int strat,
            MPI_Comm *comm)
{
    int Pivs[size], result, i;

    MPI_Allgather(&locPiv, 1, MPI_INT, &Pivs[0], size, MPI_INT, *comm);

    if (strat == 1)             // choose median of rank 0
    {
        result = Pivs[0];
    }
    else if (strat == 2)        // choose the median of the medians
    {
        qsort((void *)Pivs, size, sizeof(int), qsComp);
        result = Pivs[size/2];
    }
    else if (strat == 3)        // choose the mean of the medians
    {
        for (i = 0; i < size; i++)
        {
            result += Pivs[i];
        }
        result = result/size;
    }

    return result;
}

// Merge two arrays
void mergeArrays(int **local_data,
                 int *nLoc,
                 int sendDisp,
                 int sendCount,
                 int *rcvBuf,
                 int rcvCount,
                 int **tmp_array)
{
    int locInd, rcvInd, i, newNLoc, *tmp;

    if (sendDisp == 0)
    {   
        // Determine indicies starting points and new nLoc
        locInd = sendCount;
        rcvInd = 0;
        newNLoc = *nLoc - sendCount + rcvCount;

        // Merge arrays, reused algorithm from iterative mergesort
        for (i = 0; i < newNLoc; i++)
        {
            // if local_data ind has not hit end and either rcvInd is at end or
            //loc element is smaller than rcv element
            if ( locInd < *nLoc && (rcvInd >= rcvCount || (*local_data)[locInd] <= rcvBuf[rcvInd]) )
            {
                (*tmp_array)[i] = (*local_data)[locInd];
                locInd++;
            }
            else //rcv element is smaller than loc element
            {
                (*tmp_array)[i] = rcvBuf[rcvInd];
                rcvInd++;
            }
        }
    }
    else
    {
        // Determine indicies starting points and new nLoc
        locInd = 0;
        rcvInd = 0;
        newNLoc = *nLoc - sendCount + rcvCount;

        // Merge arrays, reused algorithm from iterative mergesort
        for (i = 0; i < newNLoc; i++)
        {
            // if local_data ind has not hit end and either rcvInd is at end or
            //loc element is smaller than rcv element
            if ( locInd < sendDisp && (rcvInd >= rcvCount || (*local_data)[locInd] <= rcvBuf[rcvInd]) )
            {
                (*tmp_array)[i] = (*local_data)[locInd];
                locInd++;
            }
            else //rcv element is smaller than loc element
            {
                (*tmp_array)[i] = rcvBuf[rcvInd];
                rcvInd++;
            }
        }
    }

    // Update valaue of nLoc
    *nLoc = newNLoc;

    // Swap local_data and tmp_array
    tmp = *local_data;
    *local_data = *tmp_array;
    *tmp_array = tmp;    

    return;
}