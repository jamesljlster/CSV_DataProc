#include <stdio.h>

#include <csv_dataproc.h>

#define CSV_ROWS 9
#define CSV_COLS 9

int main(int argc, char* argv[])
{
	int iResult;
	int i, j, k;
	
	csv_t csv;

	// Checking
	if(argc < 2)
	{
		printf("Assign a path to write csv file\n");
		return -1;
	}

	iResult = csv_create(&csv, CSV_ROWS, CSV_COLS);
	if(iResult != CSV_NO_ERROR)
	{
		printf("csv_create() failed with error: %d\n", iResult);
		return -1;
	}

	k = 0;
	for(i = 0; i < CSV_ROWS; i++)
	{
		for(j = 0; j < CSV_COLS; j++)
		{
			k++;
			csv_set_value(csv, i, j, k);
		}
	}

	iResult = csv_write(csv, argv[1]);
	if(iResult != CSV_NO_ERROR)
	{
		printf("csv_write() failed with error: %d\n", iResult);
		return -1;
	}

	csv_delete(csv);

	return 0;
}
