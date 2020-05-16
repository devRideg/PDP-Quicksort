#include "qsAlgorithm.h"

// === QUICKSORT FUNCTIONS ===

// Fast insertion sorting algorithm. Used for small partitions.
// lo: lowest index of the array;
// hi: highest index of the array;
void insertionSort(int *arr,
                   const int lo,
                   const int hi)
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
int partitionLomuto(int *arr,
                    const int lo,
                    const int hi)
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
int medianOfN(int *arr,
              const int lo,
              const int hi)
{
    insertionSort(arr, lo, hi);
    return arr[(hi+lo)/2];
}

// Deterministic median of three.
// Finds the median of an array by sampling at three points.
int medianOfThree(int *arr,
                  const int lo,
                  const int hi)
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
int medianOfThreeRand(int *arr,
                      const int lo,
                      const int hi)
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
int partitionBasicHoare(int *arr,
                        const int lo,
                        const int hi)
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
int partitionHoare(int *arr,
                   const int lo,
                   const int hi)
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
int partitionHoareBalanced(int *arr,
                           const int lo,
                           const int hi)
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
void quicksortLomuto(int *arr,
                     const int lo,
                     const int hi)
{
    if (lo < hi)
    {
        int p = partitionLomuto(arr, lo, hi);
        quicksortLomuto(arr, lo, p-1);
        quicksortLomuto(arr, p+1, hi);
    }
}

// Quicksort recursion using Hoares's partitioning scheme.
void quicksortBasicHoare(int *arr,
                         const int lo,
                         const int hi)
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
void quicksortHoare(int *arr,
                    const int lo,
                    const int hi)
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

