#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


/**
 * Generates current wall time in seconds with up 
 * to nine significant digits.
 * @returns time in wall seconds
 */
double get_wall_seconds();

/**
 * Swap two dynamically allocated arrays
 * @param a first array to be swapped
 * @param b second array to be swapped
 */
void swap(int *a,
          int *b);

/**
 * Randomly shuffle the elements of an array
 * @param arr array to be shuffled
 * @param n length of array
 */
void shuffle(int *arr,
             const int n);

/**
 * Prints an array to teminal in a column.
 * @param arr array to be printed
 * @param n length of array
 */
void printArrVert(int *arr,
                  const int n);

/**
 * Prints an array to terminal horizontally.
 * @param arr array to be printed
 * @param n length of array
 */
void printArr(int *arr,
              const int n);

/**
 * Prints a subset of an arr from lo to hi inclusive.
 * @param arr array with elements to be printed
 * @param lo lower index of subset
 * @param hi upper index of subset
 */
void printSubArr(int *arr,
                 const int lo,
                 const int hi);

/**
 * Generates an ordered array of equally spaced integers of given length.
 * @param n legth of array to be generated
 * @returns address to dynamically allocated generated array
 */ 
int *generateArr(const int n);

/**
 * Generates a copy of an array.
 * @param oldArr array to be copied
 * @param n length of array
 * @returns adress to a dynamically allocated copied array
 */
int *copyArr(int *oldArr,
             const int n);

/**
 * Verify if the values of an integer array are sorted
 * @param arr array to be tested
 * @param n length of array
void verifySorted(int *arr,
                  const int n);