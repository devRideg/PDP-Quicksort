#include "qsArray.h"

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
int read_input(int **arr,
               char *input_name)
{
    int n, i;
    FILE *inputFile;

    // open file
    inputFile = fopen(input_name, "r");
    if (inputFile == NULL)
    {
        printf("Error opening file with name: %s\n", input_name);
        return -1;
    }

    // Read input array length
    fscanf(inputFile, "%d ", &n);
    if (n < 0)
    {
        printf("Error! Invalid input array length: n = %d\n", n);
        return -1;
    }

    // allocate array memory and read input data into array
    *arr = (int *) malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
    {
        fscanf(inputFile, "%d ", &(*arr)[i]);
    }

    fclose(inputFile);

    return n;
}

// Reads input files
void read_input2(int **arr,
               char *input_name,
               int n)
{
    int i;
    FILE *inputFile;

    // open file
    inputFile = fopen(input_name, "r");
    if (inputFile == NULL)
    {
        printf("Error opening file with name: %s\n", input_name);
        return;
    }

    // allocate array memory and read input data into array
    *arr = (int *) malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
    {
        fscanf(inputFile, "%d ", &(*arr)[i]);
    }

    fclose(inputFile);

    return;
}

// Write outputfile
int write_output(int *arr,
                 int n,
                 char *output_name)
{
    int i; 
    FILE *outputFile;
    
    // open file
    outputFile = fopen(output_name, "w");
    if (outputFile == NULL)
    {
        printf("Error opening file with name: %s\n", output_name);
        return -1;
    }

    // Write array to file
    for (i = 0; i < n; i++)
    {
        fprintf(outputFile, "%d ", arr[i]);
    }

    fclose(outputFile);
    return 0;
}

// Comparator function for qsort
int qsComp(const void *a,
           const void *b)
{
    return (*(int *)a - *(int *)b);
}
