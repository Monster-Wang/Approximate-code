#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "NetlistToMap.h"

#define MAX_MATRIX_SQUARE 1024


typedef struct
{
	int Col;
	int Row;
	float Matrix[MAX_MATRIX_SQUARE][MAX_MATRIX_SQUARE];

}Struct_Matrix;


class Matrix
{
public:
	//
	Matrix();
	//
	bool MatrixMove(Struct_Matrix *src, Struct_Matrix *dest);
	//
	bool MatrixMultiply(Struct_Matrix *src, Struct_Matrix *dest);
	//
	bool MatrixTensor(Struct_Matrix *src, Struct_Matrix *dest);
	//
	void StoreMatrix(char *moduleName, Struct_Matrix *src);
	//
	void MatrixPrint(Struct_Matrix *src);
	
	void ClearFile(char *moduleName);

};

#endif