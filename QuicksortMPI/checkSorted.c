/**
 * Small program to check if data is sorted in descending order
 */
#include "checkSorted.h"

int main(int argc, char *argv[])
{
    int n, i, *arr, count = 0;
    n = atoi(argv[2]);

    read_input2(&arr, argv[1], n);

    if(n < 0) return n;

    for (i = 1; i < n; i++)
    {
        if(arr[i] < arr[i-1]) count++;
    }

    if(count == 0)
    {
        printf("Array is correctly sorted in ascending order!\n");
    }
    else
    {
        printf("Array does not follow decending order for %d elements.\n", count);
    }

    free(arr);
    return 0;
}