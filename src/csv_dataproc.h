#ifndef __CSV_DATAPROC_H__
#define __CSV_DATAPROC_H__

enum CSV_RETURN_VALUE
{
	CSV_NO_ERROR		= 0,
	CSV_MEM_FAILED		= -1,
	CSV_FILE_FAILED		= -2,
	CSV_FORMAT_ERROR	= -3,
	CSV_OUT_OF_RANGE	= -4
};

typedef struct _CSV
{
	int rows;
	int cols;
	double* data;
} csv_t;

int csv_create(csv_t* csvPtr, int rows, int cols);
int csv_read(csv_t* csvPtr, const char* filePath);

int csv_set_value(csv_t csv, int row, int col, double num);
int csv_get_value(csv_t csv, int row, int col, double* numPtr);

int csv_write(csv_t csv, const char* filePath);
int csv_delete(csv_t csv);

void csv_print(csv_t csv);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif
