/**
 * Small program to check if data is sorted in descending order
 */
#include "qsArray.h"

int main(int argc, char *argv[])
{
    int n, i, *arr, count = 0;

    n = read_input(arr, argv[1]);

    if(n < 0) return n;

    for (i = 1; i < n; i++)
    {
        if(arr[i] > arr[i-1]) count++;
    }

    if(count == 0)
    {
        printf("Array is correctly sored in descending order!\n");
    }
    else
    {
        print("Array does not follow decending order for %d elements.\n", count);
    }

    free(arr);
    return 0;
}