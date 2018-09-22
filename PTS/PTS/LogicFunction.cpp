#include "LogicFunction.h"
#include <stdlib.h>
#include <math.h>

LogicFunction::LogicFunction(){

	;
}

void LogicFunction::ANDFunction(float* &src, float *x1, float *x2, float rg){
	float p0, p1, q0, q1;
	p0 = x1[0]; p1 = x1[1];
	q0 = x2[0]; q1 = x2[1];

	src[0] = (p0*q0 + p0*q1 + p1*q0)*rg + p1*q1*(1 - rg);
	src[1] = (p0*q0 + p0*q1 + p1*q0)*(1 - rg) + p1*q1*rg;
}


void LogicFunction::ORFunction(float* &src, float *x1, float *x2, float rg){
	float p0, p1, q0, q1;
	p0 = x1[0]; p1 = x1[1];
	q0 = x2[0]; q1 = x2[1];

	src[0] = p0*q0*rg + (p0*q1 + p1*q0 + p1*q1)*(1 - rg);
	src[1] = p0*q0*(1 - rg) + (p0*q1 + p1*q0 + p1*q1)*rg;
}


void LogicFunction::NOTFunction(float* &src, float *x1, float rg){
	float p0, p1;
	p0 = x1[0]; p1 = x1[1];

	src[0] = p0*(1 - rg) + p1*rg;
	src[1] = p0*rg + p1*(1 - rg);
}


void LogicFunction::NANDFunction(float* &src, float *x1, float *x2, float rg){
	float p0, p1, q0, q1;
	p0 = x1[0]; p1 = x1[1];
	q0 = x2[0]; q1 = x2[1];

	src[0] = (p0*q0 + p0*q1 + p1*q0)*(1 - rg) + p1*q1*rg;
	src[1] = (p0*q0 + p0*q1 + p1*q0)*rg + p1*q1*(1 - rg);
}


void LogicFunction::NORFunction(float* &src, float *x1, float *x2, float rg){
	float p0, p1, q0, q1;
	p0 = x1[0]; p1 = x1[1];
	q0 = x2[0]; q1 = x2[1];

	src[0] = p0*q0*(1 - rg) + (p0*q1 + p1*q0 + p1*q1)*rg;
	src[1] = p0*q0*rg + (p0*q1 + p1*q0 + p1*q1)*(1 - rg);
}


void LogicFunction::XORFunction(float* &src, float *x1, float *x2, float rg){
	float p0, p1, q0, q1;
	p0 = x1[0]; p1 = x1[1];
	q0 = x2[0]; q1 = x2[1];

	src[0] = (p0*q0 + p1*q1)*rg + (p0*q1 + p1*q0)*(1 - rg);
	src[1] = (p0*q0 + p1*q1)*(1 - rg) + (p0*q1 + p1*q0)*rg;
}


