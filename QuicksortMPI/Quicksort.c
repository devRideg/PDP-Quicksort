#include "Quicksort.h"


int main(int argc, char *argv[])
{
    double startTime;
    int root = 0, rank, size, *input_data, nGlob; 
    int *local_data, nLoc, *rcvBuf, nRcv;
    int recvcnts[size];                         // Recieve counts for gather
    int displs[size];                           // displacements for gather

    const int pivot_strat = atoi(argv[3]);      // Storing pivot strategy choise 
    char *input_name = argv[1];                 // input file name
    char *output_name = argv[2];                // output file name
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Read input input data
    if (rank == root)
    {
        nGlob = read_input(&input_data, input_name);
        startTime = MPI_Wtime();
    }

    // Broadcast global array size
    MPI_Bcast(&nGlob, 1, MPI_INT, root, MPI_COMM_WORLD);

    // Calculate local array and scatter input data
    nLoc = nGlob / size;

    if (rank == root)
    {
        nLoc = nLoc + (nGlob % size);
    }

    local_data = (int *) malloc(1.5 * nLoc * sizeof(int));
    nRcv = nLoc;
    rcvBuf = (int *) malloc(nLoc * sizeof(int));

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

    // Perform initial local sorting 
    quicksortHoare(local_data, 0, nLoc-1);

    // If more than one processors are used perform parallel qSort
    if (size > 1)
    {
        /* implement parallel qSort */
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

        if (write_output(input_data, nGlob, output_name) < 0)
        {
            return -1;
        }
    }

    free(rcvBuf);
    free(local_data);
    free(input_data);
    MPI_Finalize();
    return 0;
}


/* ----------------------- OLD CODE!!!! ------------------------------------

// === PARALLELIZED QUICKSORT ===


// Creates threads and assigns their workload.
void quicksortParall(int* arr, const int lo, const int hi)
{
    // Reset argument array.
    for (int i=0; i<16; i++)
    {
        args[i].thread = 0;
        args[i].threadID = 0;
        args[i].arr = NULL;
        args[i].lo = 0;
        args[i].hi = 0;
    }

    // Partition array with low risk of unbalanced load.
    preprocess(arr, lo, hi, 0);

    // For all leaf partitions let a thread perform sequential quicksort.
    for(int i=0; i<threadCounter; i++)
        pthread_create(&args[i].thread, NULL, parallHelper, (void*)&args[i]);
    
    // Join threads and reset thread counter.
    void* status;
    for(int i=0; i<threadCounter; i++)
        pthread_join(args[i].thread, &status);

    threadCounter = 0;
    
}

// Used for finding the optimal array size to switch to insertion sort.
int cutoff = 0;
void quicksortInsertTest(int* arr, const int lo, const int hi)
{
    if (lo < hi)
    {
        // If array is small, use insertion sort.
        if (hi-lo < cutoff)
        {
            insertionSort(arr, lo, hi);
            return;
        }
        int p = partitionHoare(arr, lo, hi);
        quicksortInsertTest(arr, lo, p);
        quicksortInsertTest(arr, p+1, hi);
    }
}

// Used to approximate what size of an array it is more beneficial
// to use insertion sort than to keep recursing.
void optimalInsertCutoff()
{
    const int N = 1000000;
    int* arr;
    int* arrCopy;
    double startTime;
    double insertTime;
    double noInsertTime;
    
    // Generate two identical arrays.
    arr = generateArr(N);
    shuffle(arr, N);
    arrCopy = copyArr(arr, N);

    // Do a measurement where no insertion sort is used
    cutoff = 0;
    startTime = get_wall_seconds();
    quicksortInsertTest(arr, 0, N-1);
    noInsertTime = get_wall_seconds() - startTime;
    arr = arrCopy;
    arrCopy = copyArr(arr, N);

    // Compare to when insertion sort is used for different cutoff values and write results to a file.
    FILE *outputFile = fopen("VaryingInsertionCutoffTimings.txt", "w");
    fprintf(outputFile, "nElements,cutoff,noInsertTime,InsertTime,speedup\n");
    for (int i=1; i<3; i++)
    {
        for (int j=1; j<6; j++)
        {
            cutoff = 2*j*10*pow(10, i-1);
            printf("Cutoff: %i\n", cutoff);

            startTime = get_wall_seconds();
            if (cutoff == 1000000)
                printf("Hello!");
            quicksortInsertTest(arr, 0, N-1);
            insertTime = get_wall_seconds() - startTime;
            verifySorted(arr, N);
            fprintf(outputFile, "%i,%i,%3.6f,%3.6f,%3.6f\n", N, cutoff, noInsertTime, insertTime, noInsertTime/insertTime);
            
            free(arr);
            arr = arrCopy;
            arrCopy = copyArr(arr, N);

        }
    }
}

// Used to perform timings between different versions of quicksort.
// Writes a result file.
void doTimings()
{
    
    int* arr1;
    int* arr2;
    int N;

    double startTime;
    double seqTime;
    double parTime;

    FILE *outputFile = fopen("run1.txt", "w");
    fprintf(outputFile, "nElements,seqTime,parTime,speedup\n");
    
    // from 1000 to 100000000
    for (int i=1; i<6; i++)
    {
        for (int j=1; j<6; j++)
        {
            // Update N.
            N = (2*j)*1000*pow(10, i-1);
            
            // Do first timing.
            printf("*** Sorting %i elements\n", N);
            printf(">>> Sorting using sequential quicksort...\n");
            arr1 = generateArr(N);
            shuffle(arr1, N);
            arr2 = copyArr(arr1, N);
            printf("Original: ");
            verifySorted(arr1, N);
            startTime = get_wall_seconds();
            quicksortHoare(arr1, 0, N-1);
            seqTime = get_wall_seconds() - startTime;
            printf("Sorted:   ");
            verifySorted(arr1, N);
            
            // Do second timing.
            printf(">>> Sorting using parallelized quicksort...\n");
            printf("Original: ");
            verifySorted(arr2, N);
            startTime = get_wall_seconds();
            quicksortParall(arr2, 0, N-1);
            parTime = get_wall_seconds()-startTime;
            printf("Sorted:   ");
            verifySorted(arr2, N);
            
            // Print to output file.
            fprintf(outputFile, "%i,%3.6f,%3.6f,%3.6f\n", N, seqTime, parTime, seqTime/parTime);
            printf("Done\n\n");
            free(arr1);
            free(arr2);
            arr1 = NULL;
            arr2 = NULL;
        }
    }
    fclose(outputFile);
}

// Main driver code.
int main(int argc, char *argv[])
{
    int* arr1;
    int* arr2;

    // Testing parallel sorting.
    int N = 50000000;
    printf("\n\n>>> PARALLEL TIMINGS:\n");
    printf("    Using array size: %i.\n", N);

    arr1 = generateArr(N);
    shuffle(arr1, N);
    arr2 = copyArr(arr1, N);

    printf("arr1: ");
    verifySorted(arr1, N);
    printf("arr2: ");
    verifySorted(arr2, N);
    printf("\n");

    double startTime;
    printf("Sorting arr1 using sequential quicksort.\n");
    printf("Original arr1: ");
    verifySorted(arr1, N);
    startTime = get_wall_seconds();
    quicksortHoare(arr1, 0, N-1);
    printf("Sorting time: %3.6f seconds.\n", get_wall_seconds() - startTime);
    printf("Verifying...\n");
    printf("Sorted arr1:   ");
    verifySorted(arr1, N);
    printf("\n\n");
    free(arr1);

    printf("Sorting arr2 using parallelized quicksort.\n");
    printf("Original arr2: ");
    verifySorted(arr2, N);
    startTime = get_wall_seconds();
    quicksortParall(arr2, 0, N-1);
    printf("Sorting time: %3.6f seconds.\n", get_wall_seconds() - startTime);
    printf("Verifying...\n");
    printf("Sorted arr2:   ");
    verifySorted(arr2, N);
    printf("\n\n");
    free(arr2);

    pthread_exit(NULL);
}
*/