#include "Quicksort.h"


int main(int argc, char *argv[])
{
    double startTime;
    int root = 0, rank, size, *input_data, nGlob, buffersize; 
    int *local_data = NULL, nLoc, *rcvBuf = NULL, nRcv, *tmp_array = NULL;

    const int pivot_strat = atoi(argv[3]);      // Storing pivot strategy choise 
    char *input_name = argv[1];                 // input file name
    char *output_name = argv[2];                // output file name
    MPI_Comm comm = MPI_COMM_WORLD;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int recvcnts[size];                         // Recieve counts for gather
    int displs[size];                           // displacements for gather

    // Read input input data
    if (rank == root)
    {
        nGlob = read_input(&input_data, input_name);
        startTime = MPI_Wtime();
    }

    // Broadcast global array size
    MPI_Bcast(&nGlob, 1, MPI_INT, root, MPI_COMM_WORLD);

    // Calculate local array and scatter input data
    nLoc = create_nLoc(nGlob, size, rank);

    // Calculate appropriate buffersize for send and recieve buffers
    buffersize = nGlob / ((int) log2((double) size));
    if (nGlob / size < 32)
    {
        buffersize *= 4;
    }

    printf("nLoc = %d, pID:%d\n", nLoc, rank);
    local_data = (int *) malloc(buffersize * sizeof(int));
    nRcv = nLoc;
    rcvBuf = (int *) malloc(buffersize *sizeof(int));

    // Gather local data array sizes
    MPI_Allgather(&nLoc, 1, MPI_INT, &recvcnts[0], 1, MPI_INT, MPI_COMM_WORLD);

    // Calculate displacements in output array
    displs[0] = 0;
    for (int i = 1; i < size; i++)
    {
        displs[i] = displs[i-1] + recvcnts[i-1];
    }

    // scatter input data to processes
    MPI_Scatterv(&input_data[0], &recvcnts[0], &displs[0], MPI_INT,
                &local_data[0], nLoc, MPI_INT, root, MPI_COMM_WORLD);

    //printArr(displs, size);
    //printArr(local_data, nLoc);
    // Perform initial local sorting 
    qsort((void *)local_data, nLoc, sizeof(int), qsComp);
    
    // If more than one processors are used perform parallel qSort
    if (size > 1)
    {
        tmp_array = (int *) malloc(buffersize * sizeof(int));
        nLoc = parQSort(&local_data, rcvBuf, &tmp_array, nLoc, &comm, pivot_strat);
    }

    // Gather local data array sizes
    MPI_Allgather(&nLoc, 1, MPI_INT, &recvcnts[0], 1, MPI_INT, MPI_COMM_WORLD);

    // Calculate displacements in output array
    displs[0] = 0;
    for (int i = 1; i < size; i++)
    {
        displs[i] = displs[i-1] + recvcnts[i-1];
    }
    
    // Gather data at root
    MPI_Gatherv(&local_data[0], nLoc, MPI_INT, &input_data[0], &recvcnts[0],
                &displs[0], MPI_INT, root, MPI_COMM_WORLD);

    // write output file and print execution time
    if (rank == root)
    {
        printf("%f", MPI_Wtime() - startTime);

        write_output(input_data, nGlob, output_name);

	free(input_data);
    }

    free(tmp_array);
    free(rcvBuf);
    free(local_data);
    printf("after freeing arays, pID:%d\n", rank);
    MPI_Finalize();
    return 0;
}
      
