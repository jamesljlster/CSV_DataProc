#include <stdlib.h>
#include <stdio.h>

#include "csv_dataproc.h"

int csv_delete(csv_t csv)
{
	if(csv.data != NULL)
	{
		free(csv.data);
	}
	
	return CSV_NO_ERROR;
}

void csv_print(csv_t csv)
{
	int i, j;

	for(i = 0; i < csv.rows; i++)
	{
		for(j = 0; j < csv.cols; j++)
		{
			printf("%lf", csv.data[i * csv.cols + j]);

			if(j == csv.cols - 1)
			{
				printf("\n");
			}
			else
			{
				printf(", ");
			}
		}
	}
}

