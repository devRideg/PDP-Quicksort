#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

// For timings.
double get_wall_seconds()
{
  struct timespec tspec;
  clock_gettime(CLOCK_REALTIME, &tspec);
  
  double seconds = tspec.tv_sec + (double)tspec.tv_nsec/(double)1000000000;
  return seconds;
}

// Swaps two integers.
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Shuffles an array using rand().
// arr: array.
// n:   array size.
void shuffle(int* arr, const int n)
{
    // Set seed for rand().
    srand(time(NULL));

    // Shuffle array.
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

// Prints an array to teminal in a column.
void printArrVert(int* arr, const int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("idx %i: %i\n", i, arr[i]);
    }
}

// Prints an array to terminal horizontally.
void printArr(int* arr, const int n)
{
    printf("[");
    for (int i = 0; i < n-1; i++)
    {
        printf("%i, ", arr[i]);
    }
    printf("%i]\n", arr[n-1]);
}

// Prints a subset of an arr from lo to hi inclusive.
void printSubArr(int* arr, const int lo, const int hi)
{
    printf("[");
    for (int i = lo; i < hi; i++)
    {
        printf("%i, ", arr[i]);
    }
    printf("%i]\n", arr[hi]);
}

// Generates an ordered array [0, 1, 2, 3, ..., n-1].
int* generateArr(const int n)
{
    int* arr = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }

    return arr;
}

// Makes a copy of an array.
int* copyArr(int* oldArr, const int n)
{
    int* newArr = (int*)malloc(sizeof(int)*n);
    for (int i=0; i<n; i++)
    {
        newArr[i] = oldArr[i];
    }
    return newArr;
}

// Verifies that an array is sorted.
void verifySorted(int* arr, const int n)
{
    for (int i=1; i<n; i++)
    {
        if (arr[i]<arr[i-1])
        {
            printf("*** Array is NOT sorted. ***\n");
            return;
        }
    }
    printf("*** Array is sorted. ***\n");
}



// === QUICKSORT FUNCTIONS ===

// Fast insertion sorting algorithm. Used for small partitions.
// lo: lowest index of the array;
// hi: highest index of the array;
void insertionSort(int* arr, const int lo, const int hi)
{
    int tmp;
    int j;
    for (int i=lo+1; i<hi+1; i++)
    {
        tmp = arr[i];
        j = i-1;

        while(j>=0 && arr[j]>tmp)
        {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = tmp;
    }
}

// Lomuto partition scheme: Choose last element as pivot.
int partitionLomuto(int* arr, const int lo, const int hi)
{
    const int pivot = arr[hi];
    int i = lo;

    for (int j=lo; j<=hi; j++)
    {
        if (arr[j] < pivot)
        {
            swap(&arr[i], &arr[j]);
            i += 1;
        }
    }

    swap(&arr[i], &arr[hi]);
    return i;
}

// Deterministic median of n elements. array is a small array.
// Used when partitioning in the first levels of recursion
// when running multiple threads. Lowers risk of bad load balance.
int medianOfN(int* arr, const int lo, const int hi)
{
    insertionSort(arr, lo, hi);
    return arr[(hi+lo)/2];
}

// Deterministic median of three.
// Finds the median of an array by sampling at three points.
int medianOfThree(int* arr, const int lo, const int hi)
{
    int left = arr[lo];
    int mid = arr[(hi+lo)/2];
    int right = arr[hi];

    if (right<left)
        swap(&right, &left);
    if (mid<left)
        swap(&mid, &left);
    if (right<mid)
        swap(&right, &mid);
    
    return mid;
}


// Non-deterministic median of three. Returns median of a subset of an array.
// Note: RAND_MAX = 2147483647 on ITC1515 system so we should be safe.
// arr: array for which the median value is to be found in selected interval.
// lo:  lower interval bound.
// hi:  upper interval bound.
int medianOfThreeRand(int* arr, const int lo, const int hi)
{
    srand(time(NULL));
    int vals[3];
    int index;

    index = (rand()%(hi-lo+1))+lo;
    vals[0] = arr[index];
    index = (rand()%(hi-lo+1))+lo;
    vals[1] = arr[index];
    index = (rand()%(hi-lo+1))+lo;
    vals[2] = arr[index];

    if (vals[2]<vals[0])
        swap(&vals[2], &vals[0]);
    if (vals[1]<vals[0])
        swap(&vals[1], &vals[0]);
    if (vals[2]<vals[1])
        swap(&vals[2], &vals[1]);

    return vals[1];
}

// Basic hoare partitioning. No median of three strategy is applied.
int partitionBasicHoare(int* arr, const int lo, const int hi)
{
    // Choose middle element as pivot.
    const int pivot = arr[(hi+lo)/2];

    // Initialize indices at the ends of the array.
    int i = lo - 1;
    int j = hi + 1;

    // Repeat until indices meet.
    while (1)
    {
        // Increment i until arr[i] exceeds pivot.
        do
            i++;
        while (arr[i] < pivot);

        // Decrement j until pivot exceeds arr[j].
        do
            j--;
        while (arr[j] > pivot);

        // If indices meet, return j.
        if (i >= j)
            return j;

        // Inversion has been found. Swap current elements.
        swap(&arr[i], &arr[j]);
    }
}

// Hoare partition scheme: Choose middle element as pivot.
int partitionHoare(int* arr, const int lo, const int hi)
{
    // Choose pivot as the median of three elements.
    const int pivot = medianOfThree(arr, lo, hi);

    // Initialize indices at the ends of the array.
    int i = lo - 1;
    int j = hi + 1;

    while (1)
    {
        // Increment i until arr[i] exceeds pivot.
        do
            i++;
        while (arr[i] < pivot);

        // Decrement j until pivot exceeds arr[j].
        do
            j--;
        while (arr[j] > pivot);

        // If indices meet, return j.
        if (i >= j)
            return j;

        // Inversion has been found. Swap current elements.
        swap(&arr[i], &arr[j]);
    }

}

// Hoare's partitioning scheme: sample the array at regular intervals
// and choose their median as pivot
int partitionHoareBalanced(int* arr, const int lo, const int hi)
{
    // Choose median of any nSamples number of elements.
    const int nSamples = 50;
    const int stepsize = (hi-lo)/(nSamples-1);
    int* pivotArr = (int*)malloc(sizeof(int)*nSamples);
    pivotArr[0] = arr[lo];
    for (int i=1; i<nSamples-1; i++)
    {   
        // Sample array at regular intervals.
        pivotArr[i] = arr[lo+i*stepsize];
    }
    pivotArr[nSamples-1] = arr[hi];
    const int pivot = medianOfN(pivotArr, 0, nSamples-1);
    free(pivotArr);

    // Initialize indices at the ends of the array.
    int i = lo - 1;
    int j = hi + 1;

    while (1)
    {
        // Increment i until arr[i] exceeds pivot.
        do
            i++;
        while (arr[i] < pivot);

        // Decrement j until pivot exceeds arr[j].
        do
            j--;
        while (arr[j] > pivot);

        // If indices meet, return j.
        if (i >= j)
            return j;

        // Inversion has been found. Swap current elements.
        swap(&arr[i], &arr[j]);
    }

}

// Quicksort recursion using Lomuto's partitioning scheme.
void quicksortLomuto(int* arr, const int lo, const int hi)
{
    if (lo < hi)
    {
        int p = partitionLomuto(arr, lo, hi);
        quicksortLomuto(arr, lo, p-1);
        quicksortLomuto(arr, p+1, hi);
    }
}

// Quicksort recursion using Hoares's partitioning scheme.
void quicksortBasicHoare(int* arr, const int lo, const int hi)
{
    
    if (lo < hi)
    {
        int p = partitionBasicHoare(arr, lo, hi);
        quicksortBasicHoare(arr, lo, p);
        quicksortBasicHoare(arr, p+1, hi);
    }
}

// Quicksort recursion using Hoares's partitioning scheme with median of three
// strategy applied and utilized insertion sort for small partitions.
void quicksortHoare(int* arr, const int lo, const int hi)
{
    
    if (lo < hi)
    {
        // If array is small, use insertion sort.
        if (hi-lo < 50)
        {
            insertionSort(arr, lo, hi);
            return;
        }
        int p = partitionHoare(arr, lo, hi);
        quicksortHoare(arr, lo, p);
        quicksortHoare(arr, p+1, hi);
    }
}





// === PARALLELIZED QUICKSORT ===

// Struct used to call parallHelper.
typedef struct threadData
{
    pthread_t thread;
    int threadID;
    int* arr;
    int lo;
    int hi;
} threadData_t;

// Intermediate step to call sequential quicksort with a pthread.
void* parallHelper(void* args)
{
    threadData_t* data = (threadData_t*)args;
    int* arr = (*data).arr;
    int lo = (*data).lo;
    int hi = (*data).hi;
    quicksortHoare(arr, lo, hi);
    pthread_exit(NULL);
}

// Preprocess by taking more samples to lower the risks of unbalanced load between threads.
// the global args array holds a threadData_t struct for each thread to be used later by quicksortParall.
const int maxLevel = 4;
int threadCounter = 0;
threadData_t args[16];
void preprocess(int* arr, const int lo, const int hi, const int level)
{
    if (level < maxLevel)
    {
        int p = partitionHoareBalanced(arr, lo, hi);
        preprocess(arr, lo, p, level+1);
        preprocess(arr, p+1, hi, level+1);
    }
    else
    {
        threadData_t data;
        data.arr = arr;
        data.lo = lo;
        data.hi = hi;
        data.threadID = threadCounter;

        args[threadCounter] = data;
        threadCounter++;
    }
    
}

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