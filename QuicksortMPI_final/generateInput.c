#include "qsArray.h"

int main(int argc, char *argv[])
{
  char *fileName = argv[1];
  int n = atoi(argv[2]);

  int *arr = generateArr(n);

  shuffle(arr, n);
  int *outputarr = (int *)malloc((n+1)*sizeof(int));
  outputarr[0] = n;
 
  for (int i = 1; i < n+1; i++)
  {
    outputarr[i] = arr[i-1];
  }
  
  write_output(outputarr, n+1, fileName);

  free(outputarr);
  free(arr);
  return 0;
}
