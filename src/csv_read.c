#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "csv_dataproc.h"
#include "csv_strproc.h"

#include "debug.h"

int csv_append_number(double** dst, int* dstLen, double num);

int csv_read(csv_t* csvPtr, const char* filePath)
{
	int iResult;
	int retValue = CSV_NO_ERROR;
	char tmpRead;
	double tmpDecode;

	int matLen = 0;
	int matRows = -1, matCols = -1, tmpCols = 0;
	double* matrix = NULL;

	str_t readBuffer;
	const char* str;

	FILE* fileRead = NULL;

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
	while(!feof(fileRead))
	{
		// Read a character
		tmpRead = str_get_char(fileRead, READ_ALL);
		if(tmpRead < 0)
			break;

		// Processing character
		if(tmpRead == ',' || tmpRead == LF)
		{
			// Decode buffer to double value
			str = str_getbuf(&readBuffer);
			if(str == NULL)
			{
				tmpDecode = 0;
			}
			else
			{
				tmpDecode = strtod(str, NULL);
				if(errno == ERANGE)
				{
					retValue = CSV_FILE_FAILED;
					goto ERR;
				}
				else
				{
					tmpCols++;
					
					// Clear read buffer
					str_clean(&readBuffer);
				}

				// Append decoded number to matrix
				iResult = csv_append_number(&matrix, &matLen, tmpDecode);
				if(iResult != CSV_NO_ERROR)
				{
					retValue = iResult;
					goto ERR;
				}

				// Check end of line
				if(tmpRead == LF)
				{
					if(matCols = -1)
					{
						matCols = tmpCols;
					}
					else
					{
						if(matCols != tmpCols)
						{
							retValue = CSV_FORMAT_ERROR;
							goto ERR;
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

	// Assign values
	csvPtr->data = matrix;
	csvPtr->rows = matRows;
	csvPtr->cols = matCols;

	goto RET;

ERR:
	if(matrix != NULL)
		free(matrix);

RET:
	if(fileRead != NULL)
		fclose(fileRead);
	
	str_clean(&readBuffer);
	
	log("exit");

	return retValue;
}

int csv_append_number(double** dstPtr, int* lenPtr, double num)
{
	int retValue = CSV_NO_ERROR;
	void* allocTmp = NULL;

	log("enter");

	allocTmp = realloc(*dstPtr, *lenPtr * (sizeof(double) + 1));
	if(allocTmp == NULL)
	{
		retValue = CSV_MEM_FAILED;
	}
	else
	{
		*dstPtr = allocTmp;
		*lenPtr += 1;
		(*dstPtr)[*lenPtr - 1] = num;
	}

	log("exit");

	return retValue;
}

