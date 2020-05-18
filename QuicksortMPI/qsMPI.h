/**
 * qsMPI contains functions related to the parallel implementation of quick sort
 * using MPI.
 * 
 * Written for assignment 3 of the PDP course.
 * Authors - Dennis Jensen and Johan Rideg 2020.
 */
#include "qsArray.h"
#include <mpi.h>

/**
 * Main parallel quick sort function. Splits sorted arrays between processors 
 * and the splits the given communicator into two new communicators and recursively
 * calls itself until communicator size is 1. 
 * 
 * Caution; allocated memory for local_data array much be larger than 
 * initial block of data. The same is applicable for tmp_array.
 * @param local_data pointer to array containing current local values
 * @param rcvBuf buffer for recieving data
 * @param tmp_array pointer to array used for merging local values and recieved values
 * @param nLoc pointer to current number of elements in local_data array
 * @param comm pointer to current MPI communicator
 * @param strat value for choosing pivot strategy
 */ 
void parQSort(int **local_data,
              int *rcvBuf,
              int **tmp_array,
              int nLoc,
              MPI_Comm *comm,
              const int strat);

/**
 * Find the pivot value for processors in a communicator using a given 
 * strategy. 
 * 
 * Implemented pivot strategies:
 * 1 - Use median of value of processor with rank 0.
 * 2 - Use the median value of the median values of all processors.
 * 3 - Use the mean value of all the median values of all processors.
 * @param locPiv median value for this processor
 * @param size size of current communicator
 * @param rank rank in current communicator
 * @param strat value for choosing pivot strategy
 * @param current communiucator
 * @returns choosen pivot value
 */ 
int findPiv(int locPiv,
            int size,
            int rank,
            int strat,
            MPI_Comm *comm);

/**
 * Merge one entire array and part of one array into one sorted array.
 * 
 * Caution; the local_data array and tmp_array array must be large enough 
 * to store the merged array.
 * @param local_data pointer to array with local data
 * @param sendDisp index of first value that was sent to other processor
 * @param sendCount number of values that were sent to other prosessor
 * @param rcvBuf recieve buffer
 * @param rcvCount number of values recieved from other processor
 * @param tmp_array pointer to temporary array for merging
 */
void mergeArrays(int **local_data,
                 int *nLoc,
                 int sendDisp,
                 int sendCount,
                 int *rcvBuf,
                 int rcvCount,
                 int **tmp_array);