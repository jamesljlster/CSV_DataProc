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
	double* dataBak;
	double* data;
	int enableHeader;
	char** header;
} *csv_t;

#ifdef __cplusplus
extern "C" {
#endif

int csv_create(csv_t* csvPtr, int rows, int cols);
int csv_read(csv_t* csvPtr, const char* filePath);
int csv_clone(csv_t* csvPtr, csv_t src);

int csv_set_value(csv_t csv, int row, int col, double num);
double csv_get_value(csv_t csv, int row, int col);
double* csv_get_value_ptr(csv_t csv, int row, int col);

int csv_get_rows(csv_t csv);
int csv_get_cols(csv_t csv);

int csv_write(csv_t csv, const char* filePath);
int csv_delete(csv_t csv);

void csv_normalize(csv_t csv, int targetColumn, double targetMin, double targetMax);
void csv_denormalize(csv_t csv, int targetColumn);

void csv_print(csv_t csv);

int csv_set_header(csv_t csv, int col, char* header);
void csv_set_enable_header(csv_t csv, int enableHeader);

#ifdef __cplusplus
}
#endif

#endif
