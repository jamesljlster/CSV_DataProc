#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "csv_dataproc.h"
#include "csv_strproc.h"

#include "debug.h"

int csv_append_number(float** dst, int* dstLen, float num);

int csv_read(csv_t* csvPtr, const char* filePath)
{
	int iResult, stopParse;
	int retValue = CSV_NO_ERROR;
	char tmpRead;
	float tmpDecode;
	char* tmpPtr;

	int matLen = 0;
	int matRows = -1, matCols = -1, tmpCols = 0;
	float* matrix = NULL;

	str_t readBuffer;
	const char* str;

	FILE* fileRead = NULL;

	csv_t tmpCsv = NULL;

	log("enter");

	// Zero memory
	str_zero_mem(&readBuffer);

	// Open file
	fileRead = fopen(filePath, "rb");
	if(fileRead == NULL)
	{
		retValue = CSV_FILE_FAILED;
		goto RET;
	}

	// Read file
	stopParse = 0;
	while(!stopParse)
	{
		// Read a character
		tmpRead = str_get_char(fileRead, READ_ALL);
		if(tmpRead < 0)
		{
			stopParse = 1;
		}

		// Processing character
		if(tmpRead == ',' || tmpRead == LF || feof(fileRead))
		{
			// Decode buffer to float value
			str = str_getbuf(&readBuffer);
			if(str == NULL)
			{
				tmpDecode = 0;
			}
			else
			{
				log("Decoding: %s", str);
				tmpDecode = strtod(str, &tmpPtr);
				if(str == tmpPtr)
				{
					log("Failed to decode: %s", str);
					retValue = CSV_FILE_FAILED;
					goto RET;
				}
				else
				{
					log("Decoded: %lf", tmpDecode);
					tmpCols++;

					// Clear read buffer
					str_clean(&readBuffer);
				}

				// Append decoded number to matrix
				iResult = csv_append_number(&matrix, &matLen, tmpDecode);
				if(iResult != CSV_NO_ERROR)
				{
					retValue = iResult;
					goto RET;
				}

				// Check end of line
				if(tmpRead == LF || feof(fileRead))
				{
					if(matCols == -1)
					{
						matCols = tmpCols;
					}
					else
					{
						if(matCols != tmpCols)
						{
							retValue = CSV_FORMAT_ERROR;
							goto RET;
						}
					}

					tmpCols = 0;
				}
			}
		}
		else
		{
			// Append char to read buffer
			str_append(&readBuffer, tmpRead);
		}
	}

	// Find row count
	matRows = matLen / matCols;

	// Create csv
	iResult = csv_create(&tmpCsv, matRows, matCols);
	if(iResult != CSV_NO_ERROR)
	{
		retValue = iResult;
		goto RET;
	}
	else
	{
		// Clone matrix
		memcpy(tmpCsv->data, matrix, matRows * matCols * sizeof(float));
		memcpy(tmpCsv->dataBak, matrix, matRows * matCols * sizeof(float));
	}

	log("matRows: %d", matRows);
	log("matCols: %d", matCols);
	#ifdef DEBUG
	int i, j;
	printf("Matrix: \n");
	for(i = 0; i < matRows; i++)
	{
		for(j = 0; j < matCols; j++)
		{
			printf("%lf", matrix[i * matCols + j]);
			if(j == matCols - 1)
			{
				printf("\n");
			}
			else
			{
				printf(",");
			}
		}
	}
	#endif

	*csvPtr = tmpCsv;

RET:
	if(matrix != NULL)
		free(matrix);

	if(fileRead != NULL)
		fclose(fileRead);

	str_clean(&readBuffer);

	log("exit");

	return retValue;
}

int csv_append_number(float** dstPtr, int* lenPtr, float num)
{
	int retValue = CSV_NO_ERROR;
	int tmpLen;
	void* allocTmp = NULL;

	log("enter");
	log("Append: %lf", num);

	tmpLen = *lenPtr;
	allocTmp = realloc(*dstPtr, (tmpLen + 1) * sizeof(float));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
	}
	else
	{
		(*dstPtr) = allocTmp;
		(*lenPtr) = tmpLen + 1;
		(*dstPtr)[tmpLen] = num;
		log("Appended: %lf", (*dstPtr)[tmpLen]);
	}

	#ifdef DEBUG
	int i;
	printf("\n");
	printf("Array: ");
	for(i = 0; i < (*lenPtr); i++)
	{
		printf("%lf, ", (*dstPtr)[i]);
	}
	printf("\n\n");
	#endif

	log("exit");

	return retValue;
}

