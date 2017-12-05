#include <stdio.h>

#include <csv_dataproc.h>

int main(int argc, char* argv[])
{
	int i;
	int iResult;
	csv_t csv;

	if(argc < 2)
	{
		printf("Assign a csv file to run the program\n");
		return -1;
	}

	iResult = csv_read(&csv, argv[1]);
	if(iResult != CSV_NO_ERROR)
	{
		printf("csv_read() failed with error: %d\n", iResult);
		return -1;
	}

	printf("Origin dataset:\n");
	csv_print(csv);
	printf("\n");

	// Normalize
	csv_normalize(csv, 0, 0, 1);

	// Print normalized dataset
	printf("Normalized column 0:\n");
	csv_print(csv);
	printf("\n");

	// Denormalize
	csv_denormalize(csv, 0);

	// Print demormalized dataset
	printf("Denormalize:\n");
	csv_print(csv);
	printf("\n");

	csv_delete(csv);

	return 0;
}
