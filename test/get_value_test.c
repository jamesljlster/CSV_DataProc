#include <stdio.h>

#include <csv_dataproc.h>

int main(int argc, char* argv[])
{
	int iResult;
	int i, j;
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

	for(i = 0; i < csv_get_rows(csv); i++)
	{
		for(j = 0; j < csv_get_cols(csv); j++)
		{
			printf("%lf", csv_get_value(csv, i, j));

			if(j == csv_get_cols(csv) - 1)
			{
				printf("\n");
			}
			else
			{
				printf(", ");
			}
		}
	}

	csv_delete(csv);

	return 0;
}
