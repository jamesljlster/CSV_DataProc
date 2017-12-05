#include <stdio.h>

#include <csv_dataproc.h>

int main(int argc, char* argv[])
{
	int iResult;
	csv_t csv;
	csv_t clone;

	if(argc < 2)
	{
		printf("Assign a csv file to run a program\n");
		return -1;
	}

	iResult = csv_read(&csv, argv[1]);
	if(iResult != CSV_NO_ERROR)
	{
		printf("csv_read() failed with error: %d\n", iResult);
		return -1;
	}

	iResult = csv_clone(&clone, csv);
	if(iResult != CSV_NO_ERROR)
	{
		printf("csv_clone() failed with error: %d\n", iResult);
		return -1;
	}

	printf("Original CSV:\n");
	csv_print(csv);

	printf("Clone CSV:\n");
	csv_print(clone);

	csv_delete(csv);

	return 0;
}
