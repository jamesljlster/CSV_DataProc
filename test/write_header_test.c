#include <stdio.h>

#include <csv_dataproc.h>

#define CSV_ROWS 9
#define CSV_COLS 9

#define BUF_LEN	100

int main(int argc, char* argv[])
{
	int iResult;
	int i, j, k;

	char buffer[BUF_LEN] = {0};

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

	// Enable header
	csv_set_enable_header(csv, 1);

	// Set header
	for(i = 0; i < CSV_COLS; i++)
	{
		sprintf(buffer, "Header %d", i);
		iResult = csv_set_header(csv, i, buffer);
		if(iResult != CSV_NO_ERROR)
		{
			printf("csv_set_header() failed with error: %d\n", iResult);
			return -1;
		}
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
