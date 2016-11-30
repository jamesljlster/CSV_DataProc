#include <stdlib.h>
#include <stdio.h>

#include "csv_dataproc.h"

#include "debug.h"

int csv_set_value(csv_t csv, int row, int col, double num)
{
	// Checking
	if(row >= csv.rows || col >= csv.cols)
		return CSV_OUT_OF_RANGE;
	
	csv.data[row * csv.cols + col] = num;

	return CSV_NO_ERROR;
}

int csv_write(csv_t csv, const char* filePath)
{
	int i, j;
	int retValue = CSV_NO_ERROR;
	FILE* fileWrite = NULL;

	// Open file
	fileWrite = fopen(filePath, "w");
	if(fileWrite == NULL)
	{
		retValue = CSV_FILE_FAILED;
		goto RET;
	}

	// Write file
	for(i = 0; i < csv.rows; i++)
	{
		for(j = 0; j < csv.cols; j++)
		{
			fprintf(fileWrite, "%lf", csv.data[i * csv.cols + j]);
			if(j == csv.cols - 1)
			{
				fprintf(fileWrite, "\n");
			}
			else
			{
				fprintf(fileWrite, ", ");
			}
		}
	}

RET:
	if(fileWrite != NULL)
		fclose(fileWrite);

	return retValue;
}

int csv_create(csv_t* csvPtr, int rows, int cols)
{
	int retValue = CSV_NO_ERROR;
	void* allocTmp = NULL;

	log("enter");

	allocTmp = calloc(rows * cols, sizeof(double));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
	}
	else
	{
		csvPtr->data = allocTmp;
		csvPtr->rows = rows;
		csvPtr->cols = cols;
	}

	log("exit");

	return retValue;
}

int csv_delete(csv_t csv)
{
	log("enter");

	if(csv.data != NULL)
	{
		free(csv.data);
	}

	log("exit");
	
	return CSV_NO_ERROR;
}

void csv_print(csv_t csv)
{
	int i, j;
	
	log("enter");

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

	log("exit");
}

