#ifndef __CSV_STRPROC_H__
#define __CSV_STRPROC_H__

#include <stdio.h>

#define LF				0x0A
#define CR				0x0D
#define ASCII_MIN_SIG	0x20
#define ASCII_MAX_SIG	0x7E

enum READ_CHAR_TYPE
{
	READ_ALL,		// Read all type character
	READ_SIG		// Read signaficant character
};

typedef struct _STR
{
	char* str;
	int size;
} str_t;

#ifdef __cplusplus
extern "C" {
#endif

int str_check_sigchar(int target);

int str_zero_mem(str_t* strPtr);
int str_clean(str_t* strPtr);
int str_append(str_t* strPtr, int appendChar);

const char* str_getbuf(str_t* strPtr);

int str_get_char(FILE* fileRead, int readAction);

#ifdef __cplusplus
}
#endif

#endif
