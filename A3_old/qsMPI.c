#include "qsMPI.h"

// main paralell Quick sort algorithm
int parQSort(int **local_data,
             int *rcvBuf,
             int **tmp_array,
             int nLoc,
             MPI_Comm *comm,
             const int strat)
{
  int size, rank, locPiv, gPiv, pivInd = -1, colour, empt = 0;
  int i, targRank, sendCount, sendDisp, rcvCount = 0;
  MPI_Comm newComm;
  MPI_Status status;

  MPI_Comm_size(*comm, &size);
  MPI_Comm_rank(*comm, &rank);

  // find local pivot value
  if (nLoc > 0)
  {
    locPiv = (*local_data)[nLoc / 2];
  }
  else
  {
    locPiv = 0;
    empt = 1;
  }

  gPiv = findPiv(locPiv, strat, empt, comm);

  if (size == 1) // if size of communicator is 0, do nothing
  {
    return nLoc;
  }
  else
  {
    // Find largest element smaller than pivot
    if (nLoc > 0)
    {
      while ((*local_data)[pivInd + 1] < gPiv && pivInd < nLoc - 1)
      {
        pivInd++;
      }
    }

    // Determine indicies and send counts
    if (rank < size / 2)
    {
      targRank = rank + size / 2;
      sendDisp = pivInd + 1;
      sendCount = nLoc - sendDisp;
      colour = 2;
    }
    else
    {
      targRank = rank - size / 2;
      sendDisp = 0;
      sendCount = pivInd + 1;
      colour = 1;
    }
    if (nLoc == 0)
    {
      sendCount = 0;
    }


    MPI_Sendrecv(&sendCount, 1, MPI_INT, targRank, rank + 1,
                 &rcvCount, 1, MPI_INT, targRank, targRank + 1, *comm, &status);

    MPI_Sendrecv(&(*local_data)[sendDisp], sendCount, MPI_INT, targRank, rank,
                 &rcvBuf[0], rcvCount, MPI_INT, targRank, targRank, *comm, &status);

    mergeArrays(local_data, &nLoc, sendDisp, sendCount, rcvBuf, rcvCount, tmp_array);


    /* Split communicator into two new communicators */
    MPI_Comm_split(*comm, colour, rank, &newComm);

    /* then recursively call parQSort again with new comm */
    nLoc = parQSort(local_data, rcvBuf, tmp_array, nLoc, &newComm, strat);
  }

  MPI_Comm_free(&newComm);
  return nLoc;
}

// Find the pivot elemenmt based on given strategy
int findPiv(int locPiv,
            int strat,
            int empt,
            MPI_Comm *comm)
{
  int i, j = 0, size, rank, result = 0, root = 0;
  MPI_Comm_size(*comm, &size);
  MPI_Comm_rank(*comm, &rank);
  int empts[size];     // = (int *) malloc(size*sizeof(int));
  int Pivs[size];      // = (int *) malloc(size*sizeof(int));
  int validPivs[size]; // = (int *) malloc(size*sizeof(int));

  MPI_Allgather(&locPiv, 1, MPI_INT, &Pivs[0], 1, MPI_INT, *comm);
  MPI_Allgather(&empt, 1, MPI_INT, &empts[0], 1, MPI_INT, *comm);

  // Check which medians comes from processors with "empty" arrays
  for (i = 0; i < size; i++)
  {
    if (empts[i] == 0)
    {
      validPivs[j] = Pivs[i];
      j++;
    }
  }
  // j now indicates the size of validPivs

  if (j > 0)
  {
    if (strat == 1) // choose median of rank 0
    {
      result = validPivs[0];
    }
    else if (strat == 2) // choose the median of the medians
    {

      qsort((void *)validPivs, j, sizeof(int), qsComp);
      result = validPivs[j / 2];
    }
    else if (strat == 3) // choose the mean of the medians
    {
      double j2 = 1.0 / (double)j;
      for (i = 0; i < j; i++)
      {
        result += validPivs[i] * j2;
      }
      result = result;
    }
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
      if (locInd < *nLoc && (rcvInd >= rcvCount || (*local_data)[locInd] <= rcvBuf[rcvInd]))
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
      if (locInd < sendDisp && (rcvInd >= rcvCount || (*local_data)[locInd] <= rcvBuf[rcvInd]))
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

// generates appropriate nLoc depending on rank
int create_nLoc(int nGlob,
                int size,
                int rank)
{
  int nLoc, rem;

  nLoc = nGlob / size;
  rem = nGlob % size;

  if (rank < rem)
  {
    nLoc++;
  }

  return nLoc;
}

// prints MPI_COMM_WORLD rank to stdout
void world_rank()
{
  int wRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &wRank);
  printf("\n=====[WORLD RANK: %d]=====\n\n", wRank);
  return;
}

// Calculates buffersizes
int calc_buffersize(int nGlob,
                    int size)
{
  int buffersize = (nGlob / size + 2) * ((int)log2((double)size));
  if (nGlob/size < 32)
  {
    buffersize *= 4;
  }
  if (buffersize == 0)
  {
    buffersize = nGlob;
  }
  return buffersize;
}

// Staggered file input for reduced memory requirements at root.
// This is actually really inefficient and input files should really
// be provided as binary files and NOT txt files to enable trivial 
// parallel I/O with MPI... >:(
// (This would also reduce file size in storage)
int *staggeredFile_read(int *nGlob,
                  int *nLoc,
                  int *buffersize,
                  char *filename)
{
  int size, rank, i, j, root = 0, root_start_ind, tmp, *local_data;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int local_sizes[size];
  FILE *pfile;
  
  // read and broadcast problem size
  if (rank == root)
  {
    pfile = fopen(filename, "r");
    fscanf(pfile, "%d ", nGlob);
    root_start_ind = ftell(pfile);
  }
  
  // broadcast global data size
  MPI_Bcast(nGlob, 1, MPI_INT, root, MPI_COMM_WORLD);

  // Special handling if size > nGlob to run serially
  if (size > *nGlob)
  {
    if (rank == root) // root recieves all inputdata
    {
      *buffersize = calc_buffersize(*nGlob, size);
      *nLoc = *nGlob;
      local_data = (int *)malloc(*nGlob * sizeof(int));
      for (i = 0; i < *nGlob; i++);
      {
        fscanf(pfile, "%d ", &local_data[i]);
      }
      fclose(pfile);
      return local_data;
    }
    else // other processors recieve zeroes values
    {
      *buffersize = 1;
      *nLoc = 0;
      local_data = (int *) malloc(sizeof(int));
      return local_data;
    }
  }

  // find local chunks
  local_sizes[0] = create_nLoc(*nGlob, size, 0);
  for (i = 1; i < size; i++)
  {
    local_sizes[i] = create_nLoc(*nGlob, size, i);
  }

  // set local size for this processor
  *nLoc = local_sizes[rank];

  // allocate local data array
  *buffersize = calc_buffersize(*nGlob, size);
  local_data = (int *) malloc(*buffersize * sizeof(int));

  if (rank == root)
  {
    // find first pos for rank 1
    for (i = 0; i < local_sizes[0]; i++)
    {
      fscanf(pfile, "%d ", &tmp);
    }

    // Scatter local data
    for (i = 1; i < size; i++)
    {
      for (j = 0; j < local_sizes[i]; j++)
      {
        fscanf(pfile, "%d ", &local_data[j]); 
      }
      MPI_Send(&local_data[0], local_sizes[i], MPI_INT, i, i, MPI_COMM_WORLD);
    }

    fseek(pfile, root_start_ind, SEEK_SET);

    // read local data for root
    for (j = 0; j < local_sizes[0]; j++)
    {
      fscanf(pfile, "%d ", &local_data[j]);
    }
    fclose(pfile);
  }
  else
  {
    MPI_Recv(&local_data[0], local_sizes[rank], MPI_INT, root, rank, MPI_COMM_WORLD, &status);
  }

  return local_data;
}

// Write files in a staggered manner
void staggeredFile_write(int *local_data,
                         int nLoc,
                         char *filename)
{
  int i, j, size, rank, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int local_sizes[size];
  FILE *pfile;
  MPI_Status status;

  // Gather local file sizes
  MPI_Gather(&nLoc, 1, MPI_INT, &local_sizes[0], 1, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) // if root, gather and write data
  {
    pfile = fopen(filename, "w");

    // Write root data
    for (j = 0; j < local_sizes[0]; j++)
    {
      fprintf(pfile, "%d ", local_data[j]);
    }

    // Recvieve local data and Write
    for (i = 1; i < size; i++)
    {
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Recv(&local_data[0], local_sizes[i], MPI_INT, i, i, MPI_COMM_WORLD, &status);
      
      for (j = 0; j < local_sizes[i]; j++)
      {
        fprintf(pfile, "%d ", local_data[j]);
      }
    } 

    fclose(pfile);
  }
  else // else send data to root
  {
    for (i = 1; i < size; i++)
    {
      MPI_Barrier(MPI_COMM_WORLD); //synchronize processors
      if (rank == i)
      {
      MPI_Send(&local_data[0], nLoc, MPI_INT, root, rank, MPI_COMM_WORLD);
      }
    }
  }
  
  return;
}
