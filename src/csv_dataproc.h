#ifndef __CSV_DATAPROC_H__
#define __CSV_DATAPROC_H__

enum CSV_RETURN_VALUE
{
	CSV_NO_ERROR	= 0,
	CSV_MEM_FAILED	= -1,
	CSV_FILE_FAILED	= -2
};

typedef struct _CSV
{
	int rows;
	int cols;
	double* data;
} csv;

int csv_create(csv* csvPtr, int rows, int cols);
int csv_read(csv* csvPtr, const char* filePath);
int csv_write(csv csvPtr, const char* filePath);
int csv_delete(csv csvPtr);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif
