#include <stdio.h>
#include <stdlib.h>

#include <csv_strproc.h>

int str_check_sigchar(int target)
{
	// Determint if character is signaficant
	if(target >= ASCII_MIN_SIG && target <= ASCII_MAX_SIG)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int str_clean(str_t* strPtr)
{
	if(strPtr->str != NULL)
		free(strPtr->str);

	strPtr->size = 1;

	return 0;
}

int str_get_char(FILE* fileRead, int readAction)
{	
	int iResult;
	int readCount;
	char tmpRead;
	
	// Read a character
	readCount = 1;
	iResult = fread((void*)&tmpRead, sizeof(char), readCount, fileRead);
	if(iResult != readCount)
		goto ERR;

	if(readAction == READ_ALL)
		goto RET;

	// Determint if character is signaficant
	if(str_check_sigchar(tmpRead))
		goto RET;

ERR:
	return -1;

RET:
	return tmpRead;
}

int str_append(str_t* strPtr, int appendChar)
{
	int retValue = 0;
	void* allocTmp = NULL;
	
	// Checking
	if(strPtr->size == 0)
		strPtr->size = 1;

	allocTmp = realloc(strPtr->str, sizeof(char) * (strPtr->size + 1));
	if(allocTmp == NULL)
	{
		retValue = -1;
	}
	else
	{
		strPtr->str = allocTmp;
		strPtr->size += 1;

		strPtr->str[strPtr->size - 1] = '\0';
		strPtr->str[strPtr->size - 2] = appendChar;
	}

	return retValue;
}

const char* str_getbuf(str_t* strPtr)
{
	return (const char*)strPtr->str;
}

