#include <stdio.h>

#include <csv_dataproc.h>

#define TEST_CSV_PATH	"./test.csv"

int main(int argc, char* argv[])
{
	int iResult;
	csv_t csv;

	while(1)
	{
		iResult = csv_read(&csv, TEST_CSV_PATH);
		if(iResult != CSV_NO_ERROR)
		{
			printf("csv_read() failed with error: %d\n", iResult);
			return -1;
		}

		csv_delete(csv);
	}

	return 0;
}
