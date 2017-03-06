#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "csv_dataproc.h"

#include "debug.h"

int csv_set_header(csv_t csv, int col, char* header)
{
	int retValue = CSV_NO_ERROR;
	void* allocTmp = NULL;

	// Memory allocation
	allocTmp = calloc(strlen(header) + 1, sizeof(char));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
		goto RET;
	}
	else
	{
		memcpy(allocTmp, header, strlen(header) * sizeof(char));
		csv->header[col] = allocTmp;
	}

RET:
	return retValue;
}

void csv_set_enable_header(csv_t csv, int enableHeader)
{
	csv->enableHeader = enableHeader;
}

void csv_denormalize(csv_t csv, int targetColumn)
{
	int i;

	for(i = 0; i < csv->rows; i++)
	{
		csv->data[i * csv->cols + targetColumn] = csv->dataBak[i * csv->cols + targetColumn];
	}
}

void csv_normalize(csv_t csv, int targetColumn, double targetMin, double targetMax)
{
    int i;

    double dataMin, dataMax;

    // Find Max and Min Value
    dataMin = csv->data[targetColumn];
    dataMax = csv->data[targetColumn];
    for(i = 1; i < csv->rows; i++)
    {
        if(dataMax < csv->data[i * csv->cols + targetColumn])
			dataMax = csv->data[i * csv->cols + targetColumn];
        if(dataMin > csv->data[i * csv->cols + targetColumn])
			dataMin = csv->data[i * csv->cols + targetColumn];
    }

    #ifdef DEBUG
    printf("dataMax = %lf, dataMin = %lf\n", dataMax, dataMin);
    #endif // DEBUG

    // Normalization
    for(i = 0; i < csv->rows; i++)
    {
        csv->data[i * csv->cols + targetColumn] = ((csv->dataBak[i * csv->cols + targetColumn] - dataMin) / (dataMax - dataMin)) * (targetMax - targetMin) + targetMin;
    }
}

int csv_clone(csv_t* csvPtr, csv_t src)
{
	int iResult;
	int retValue = CSV_NO_ERROR;
	csv_t tmp = NULL;

	iResult = csv_create(&tmp, src->rows, src->cols);
	if(iResult != CSV_NO_ERROR)
	{
		retValue = iResult;
		goto RET;
	}

	memcpy(tmp->data, src->data, sizeof(double) * src->rows * src->cols);
	memcpy(tmp->dataBak, src->dataBak, sizeof(double) * src->rows * src->cols);

	*csvPtr = tmp;

RET:
	return retValue;
}


int csv_get_rows(csv_t csv)
{
	return csv->rows;
}

int csv_get_cols(csv_t csv)
{
	return csv->cols;
}

double csv_get_value(csv_t csv, int row, int col)
{
	// Checking
	assert(row < csv->rows && row >= 0);
	assert(col < csv->cols && col >= 0);

	return csv->data[row * csv->cols + col];

}

double* csv_get_value_ptr(csv_t csv, int row, int col)
{
	// Checking
	assert(row < csv->rows && row >= 0);
	assert(col < csv->cols && col >= 0);

	return &(csv->data[row * csv->cols + col]);

}

int csv_set_value(csv_t csv, int row, int col, double num)
{
	// Checking
	if(row >= csv->rows || col >= csv->cols)
		return CSV_OUT_OF_RANGE;
	
	csv->data[row * csv->cols + col] = num;
	csv->dataBak[row * csv->cols + col] = num;

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
	for(i = 0; i < csv->rows; i++)
	{
		for(j = 0; j < csv->cols; j++)
		{
			fprintf(fileWrite, "%lf", csv->data[i * csv->cols + j]);
			if(j == csv->cols - 1)
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
	csv_t tmpCsv = NULL;
	void* allocTmp = NULL;

	log("enter");

	tmpCsv = malloc(sizeof(struct _CSV));
	if(tmpCsv == NULL)
	{
		retValue = CSV_MEM_FAILED;
		goto RET;
	}
	else
	{
		tmpCsv->rows = rows;
		tmpCsv->cols = cols;
		tmpCsv->enableHeader = 0;
	}

	allocTmp = calloc(rows * cols, sizeof(double));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
		goto ERR;
	}
	else
	{
		tmpCsv->data = allocTmp;
		allocTmp = NULL;
	}

	allocTmp = calloc(rows * cols, sizeof(double));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
		goto ERR;
	}
	else
	{
		tmpCsv->dataBak = allocTmp;
		allocTmp = NULL;
	}

	allocTmp = calloc(cols, sizeof(char*));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
		goto ERR;
	}
	else
	{
		tmpCsv->header = allocTmp;
		allocTmp = NULL;
	}

	*csvPtr = tmpCsv;

	log("exit");

	goto RET;

ERR:
	if(tmpCsv != NULL)
		free(tmpCsv);

	if(allocTmp != NULL)
		free(allocTmp);

RET:
	return retValue;
}

int csv_delete(csv_t csv)
{
	int i;

	log("enter");
	
	if(csv != NULL)
	{
		log("free data");
		if(csv->data != NULL)
		{
			free(csv->data);
		}
		log("finish");

		log("free data backup");
		if(csv->dataBak != NULL)
		{
			free(csv->dataBak);
		}
		log("finish");

		log("free csv struct");
		free(csv);
		log("finish");

		if(csv->header != NULL)
		{
			for(i = 0; i < csv->cols; i++)
			{
				if(csv->header[i] != NULL)
				{
					free(csv->header[i]);
				}
			}
			free(csv->header);
		}
	}

	log("exit");
	
	return CSV_NO_ERROR;
}

void csv_print(csv_t csv)
{
	int i, j;
	
	log("enter");

	for(i = 0; i < csv->rows; i++)
	{
		for(j = 0; j < csv->cols; j++)
		{
			printf("%lf", csv->data[i * csv->cols + j]);

			if(j == csv->cols - 1)
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

