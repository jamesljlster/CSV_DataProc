#include <stdio.h>
#include <stdlib.h>

#include "csv_strproc.h"

#ifdef DEBUG
#undef DEBUG
#endif

#include "debug.h"

int str_zero_mem(str_t* strPtr)
{
	strPtr->str = NULL;
	strPtr->size = 0;

	return 0;
}

int str_check_sigchar(int target)
{
	log("enter");

	// Determint if character is signaficant
	if(target >= ASCII_MIN_SIG && target <= ASCII_MAX_SIG)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	log("exit");
}


int str_clean(str_t* strPtr)
{
	log("enter");

	if(strPtr->str != NULL)
	{
		free(strPtr->str);
		strPtr->str = NULL;
	}

	strPtr->size = 1;
	
	log("exit");

	return 0;
}

int str_get_char(FILE* fileRead, int readAction)
{	
	int iResult;
	int readCount;
	char tmpRead;

	log("enter");
	
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
	
	log("exit");

ERR:
	return -1;

RET:
	return tmpRead;
}

int str_append(str_t* strPtr, int appendChar)
{
	int retValue = 0;
	void* allocTmp = NULL;
	
	log("enter");

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

	log("exit");

	return retValue;
}

const char* str_getbuf(str_t* strPtr)
{
	log("enter");
	log("exit");

	return (const char*)strPtr->str;
}

