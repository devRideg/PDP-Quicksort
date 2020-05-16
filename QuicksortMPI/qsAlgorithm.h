#include "qsArray.h"

// === QUICKSORT FUNCTIONS ===

/**
 * Fast insertion sorting algorithm. Used for small partitions.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 */
void insertionSort(int *arr,
                   const int lo,
                   const int hi);

/**
 * Lomuto partition scheme: Choose last element as pivot.
 * @param lowest index of the array;
 * @param hi highest index of the array;
 * @returns the number of swapped elements
 */
int partitionLomuto(int *arr,
                    const int lo,
                    const int hi);

/**
 * Deterministic median of n elements. array is a small array.
 * Used when partitioning in the first levels of recursion
 * when running multiple threads. Lowers risk of bad load balance.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 * @returns the deterministic median
 */
int medianOfN(int *arr,
              const int lo,
              const int hi);

/**
 * Deterministic median of three.
 * Finds the median of an array by sampling at three points.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 * @returns the deterministic median
 */
int medianOfThree(int *arr,
                  const int lo,
                  const int hi);

/**
 * Non-deterministic median of three. Returns median of a subset of an array.
 * Note: RAND_MAX = 2147483647 on ITC1515 system so we should be safe.
 * @param arr pointer to array for which the median value is to be found in selected interval.
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 * @returns the deterministic median
 */
int medianOfThreeRand(int *arr,
                      const int lo,
                      const int hi);

/**
 * Basic hoare partitioning. No median of three strategy is applied.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 * @returns index of middle element of array
 */
int partitionBasicHoare(int *arr,
                        const int lo,
                        const int hi);

/**
 * Hoare partition scheme: Choose middle element as pivot.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 * @returns index of middle element of array
 */
int partitionHoare(int *arr,
                   const int lo,
                   const int hi);

/**
 * Hoare's partitioning scheme: sample the array at regular intervals
 * and choose their median as pivot
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 * @returns index of middle element of array
 */
int partitionHoareBalanced(int *arr,
                           const int lo,
                           const int hi);

/** 
 * Quicksort recursion using Lomuto's partitioning scheme.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 */
void quicksortLomuto(int *arr,
                     const int lo,
                     const int hi);

/** 
 * Quicksort recursion using Hoares's partitioning scheme.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 */
void quicksortBasicHoare(int *arr,
                         const int lo,
                         const int hi);

/**
 * Quicksort recursion using Hoares's partitioning scheme with median of three
 * strategy applied and utilized insertion sort for small partitions.
 * @param arr pointer to array
 * @param lo lowest index of the array;
 * @param hi highest index of the array;
 */
void quicksortHoare(int *arr,
                    const int lo,
                    const int hi);

