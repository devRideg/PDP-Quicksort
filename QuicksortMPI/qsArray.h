#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
/**
 * Functions related to general array interactions and used for 
 * file I/O and debugging
 * 
 * Written for assignment 3 of the PDP course.
 * Authors - Dennis Jensen and Johan Rideg 2020.
 */

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
 */
void verifySorted(int *arr,
                  const int n);

/**
 * Reads input values from file with given name and allocates 
 * memory for an array to store them in
 * @param arr pointer to non-initialized input array
 * @param input_name char array of input file name
 * @returns number of imported values
 */ 
int read_input(int **arr,
               char *input_name);

/**
 * Reads input values from file with given name and number of values and
 * allocates memory for an array to store them in. 
 * Used when the number of values are not in inputfile.
 * @param arr pointer to non-initialized input array
 * @param input_name char array of input file name
 * @param n number of values in file
 */ 
void read_input2(int **arr,
               char *input_name,
               int n);

/**
 * Writes values from an array to a file with given name
 * @param arr pointer to non-initialized input array
 * @param n length of array
 * @param input_name char array of input file name
 * @returns number of imported values
 */ 
int write_output(int *arr,
                 int n,
                 char *output_name);

/**
 * Comparator function for qsort
 * @param a pointer to an int
 * @param b pointer to an int
 * @returns comparator value
 */
int qsComp(const void *a,
           const void *b);