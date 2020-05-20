#include <stdio.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{

	int size = atoi(argv[1]);
	char* output_name = argv[2];


	FILE* file;
	if (NULL == (file = fopen(output_name, "w"))) {
		perror("Couldn't open output file");
		return -1;
	}
	for (int i = size; i >= 0; i--) {
		if (0 > fprintf(file, "%i ", i)) {
			perror("Couldn't write to output file");
		}
	}
	if (0 != fclose(file)) {
		perror("Warning: couldn't close output file");
	}


	return 0;

}