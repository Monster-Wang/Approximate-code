#include "Matrix.h"
#include <stdio.h>
#include <iostream>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Matrix::Matrix()
{
	;
}

Matrix::~Matrix()
{
	;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Matrix::MatrixMove(Struct_Matrix *src, Struct_Matrix *dest)
{
	int i, j;

	if (src->Col == 0 || src->Row == 0 || src->Col > MAX_MATRIX_COL
		|| src->Row > MAX_MATRIX_ROW)
		return false;

	dest->Col = src->Col;
	dest->Row = src->Row;
	for (i = 0; i <(src->Row); i++)
		for (j = 0; j <(src->Col); j++)
			dest->Matrix[i][j] = src->Matrix[i][j];
	return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Matrix::MatrixMultiply(Struct_Matrix *src, Struct_Matrix *dest )
{
	int i, j, k;
	float temp[MAX_MATRIX_ROW][MAX_MATRIX_COL];

	if (src->Col == 0 || src->Row == 0 || src->Col > MAX_MATRIX_COL
		|| src->Row > MAX_MATRIX_ROW || dest->Col > MAX_MATRIX_COL
		|| dest->Row > MAX_MATRIX_ROW)
		return false;
	if (dest->Col == 0 && dest->Row == 0)
	{
		MatrixMove(src, dest);
		return true;
	}
	if (dest->Col == 0 || dest->Row == 0)
		return false;
	if (src->Row != dest->Col)
		return false;

	for (i = 0; i <(dest->Row); i++)
		for (j = 0; j <(src->Col); j++)
		{
			temp[i][j] = 0.0;
			for (k = 0; k <(dest->Col); k++)
				temp[i][j] += dest->Matrix[i][k] * src->Matrix[k][j];
		}

	for (i = 0; i <(dest->Row); i++)
		for (j = 0; j <(src->Col); j++)
			dest->Matrix[i][j] = temp[i][j];
	dest->Col = src->Col;

	return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Matrix::MatrixTensor(Struct_Matrix *src, Struct_Matrix *dest)
{
	int i=0, j=0, m, n;
	float Temp[MAX_MATRIX_ROW][MAX_MATRIX_COL];

	if (src->Col == 0 || src->Row == 0 
		|| src->Col > MAX_MATRIX_COL || src->Row > MAX_MATRIX_ROW 
		|| dest->Col > MAX_MATRIX_COL || dest->Row > MAX_MATRIX_ROW 
		|| src->Col*dest->Col > MAX_MATRIX_COL || src->Row*dest->Row > MAX_MATRIX_ROW)
		return false;

	if (dest->Col == 0 && dest->Row == 0)
	{
		MatrixMove(src, dest);
		return true;
	}
	if (dest->Col == 0 || dest->Row == 0)
		return false;

	for (i = 0; i <(src->Row); i++)
		for (j = 0; j <(dest->Row); j++)
			for (m = 0; m <(src->Col); m++)			
				for (n = 0; n < (dest->Col); n++)
					Temp[i*(dest->Row) + j][m*(dest->Col) + n] = src->Matrix[i][m] * dest->Matrix[j][n];
					
	for (i = 0; i <((src->Row)*(dest->Row)); i++)
		for (j = 0; j <((src->Col)*(dest->Col)); j++)
			dest->Matrix[i][j] = Temp[i][j];

	dest->Row = (src->Row)*(dest->Row);
	dest->Col = (src->Col)*(dest->Col);
	return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Matrix::StoreMatrix(char *netName, Struct_Matrix *src)
{
	FILE *fp;
	int i, j;
	char Filename[100];

	//if (src == NULL){
	//	cout << "The Matrix is NULL" << endl;
	//	return;
	//}

	strcpy(Filename, "./backup/");
	strcat(Filename, netName);
	strcat(Filename, ".txt");

	fp = fopen(Filename, "a");
	if (!fp){
		cout << "Opening the file failed" << endl;
		return;
	}
		
	//fprintf(fp, "%d %d\n", src->Row, src->Col);
	for (i = 0; i <(src->Row); i++)
	{
		for (j = 0; j <(src->Col); j++)
			fprintf(fp, "%.5f ", src->Matrix[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}

void Matrix::MatrixPrint(Struct_Matrix *src)
{
	int i, j;
	for (i = 0; i < src->Row; i++)
	{
		for (j = 0; j < src->Col; j++)
		{
			printf("%.4f  ", src->Matrix[i][j]);
		}
		printf("\n");
	}
}

void Matrix::ClearFile(char *netName){

	FILE *fp;
	char Filename[100];
	strcpy(Filename, "./backup/" );
	strcat(Filename, netName);
	strcat(Filename, ".txt");

	fp = fopen(Filename, "r");
	if (fp){
		fclose(fp);
		remove(Filename);
	}
}