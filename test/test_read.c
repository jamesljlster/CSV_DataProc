#include <stdio.h>

#include <csv_dataproc.h>

int main(int argc, char* argv[])
{
	int iResult;
	csv_t csv;
	
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

	csv_print(csv);

	csv_delete(csv);

	return 0;
}
