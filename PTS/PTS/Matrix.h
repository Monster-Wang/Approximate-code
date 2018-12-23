#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "NetlistToMap.h"


#define MAX_MATRIX_ROW 1
#define MAX_MATRIX_COL 65536


typedef struct
{
	int Col;
	int Row;
	float **Matrix;

}Struct_Matrix;



class Matrix
{
public:
	//
	Matrix();
	~Matrix();
	//
	bool MatrixMove(Struct_Matrix *src, Struct_Matrix *dest);
	//
	bool MatrixMultiply(Struct_Matrix *src, Struct_Matrix *dest);
	//
	bool MatrixTensor(Struct_Matrix *src, Struct_Matrix *dest);
	//
	void StoreMatrix(char *netName, Struct_Matrix *src);
	//
	void MatrixPrint(Struct_Matrix *src);
	//
	void ClearFile(char *netName);

};

#endif