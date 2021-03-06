#include "Quicksort.h"

int main(int argc, char *argv[])
{
    double startTime;
    int root = 0, rank, size, *input_data, nGlob, buffersize; 
    int *local_data = NULL, nLoc, *rcvBuf = NULL, *tmp_array = NULL;

    const int pivot_strat = atoi(argv[3]);      // Storing pivot strategy choise 
    char *input_name = argv[1];                 // input file name
    char *output_name = argv[2];                // output file name
    MPI_Comm comm = MPI_COMM_WORLD;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int recvcnts[size];                         // Recieve counts for gather
    int displs[size];                           // displacements for gather

    // Staggered file output, reads and distributes data, opne chunk at a time to reduce
    // memory usage
    local_data = staggeredFile_read(&nGlob, &nLoc, &buffersize, input_name);

    // Start timer
    if (rank == root)
    {
        startTime = MPI_Wtime();
    }

    // set to run serially if number of processors is larger than problem size.
    if (size > nGlob)
    {
        size = 1;
    }

    // Allocate local buffers
    tmp_array = (int*) malloc(buffersize * sizeof(int));
    rcvBuf = (int *) malloc(buffersize *sizeof(int));

    // Perform initial local sorting 
    qsort((void *)local_data, nLoc, sizeof(int), qsComp);

    // If more than one processors are used perform parallel qSort
    if (size > 1)
    {
        nLoc = parQSort(&local_data, rcvBuf, &tmp_array, nLoc, &comm, pivot_strat);
    }

    // Stop timer
    if (rank == root)
    {
        printf("%f", MPI_Wtime() - startTime);
    }

    // free local buffers
    free(tmp_array);
    free(rcvBuf);

    // Write output in a staggered manner
    if (OUTPUTFILE)
    {
        staggeredFile_write(local_data, nLoc, size, output_name);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
      
