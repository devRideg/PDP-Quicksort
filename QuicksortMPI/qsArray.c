#include "qsArray.h"

// For timings.
double get_wall_seconds()
{
  struct timespec tspec;
  clock_gettime(CLOCK_REALTIME, &tspec);
  
  double seconds = tspec.tv_sec + (double)tspec.tv_nsec/(double)1000000000;
  return seconds;
}

// Swaps two integers.
void swap(int *a,
          int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Shuffles an array using rand().
// arr: array.
// n:   array size.
void shuffle(int* arr, 
const int n)
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
void printArrVert(int *arr,
                  const int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("idx %i: %i\n", i, arr[i]);
    }
}

// Prints an array to terminal horizontally.
void printArr(int* arr, 
const int n)
{
    printf("[");
    for (int i = 0; i < n-1; i++)
    {
        printf("%i, ", arr[i]);
    }
    printf("%i]\n", arr[n-1]);
}

// Prints a subset of an arr from lo to hi inclusive.
void printSubArr(int *arr,
                 const int lo,
                 const int hi)
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
int *copyArr(int *oldArr,
             const int n)
{
    int* newArr = (int*)malloc(sizeof(int)*n);
    for (int i=0; i<n; i++)
    {
        newArr[i] = oldArr[i];
    }
    return newArr;
}

// Verifies that an array is sorted.
void verifySorted(int *arr,
                  const int n)
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

// Reads input files
int read_input(int *arr, char *input_name)
{
    file
}
